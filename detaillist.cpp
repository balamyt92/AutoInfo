#include "detaillist.h"
#include "ui_detaillist.h"

DetailList::DetailList(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DetailList)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Window);
    this->setWindowTitle(tr("Детали"));

    inLinked = false;

    model = new QSqlTableModel(this);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->setTable("carendetailnames");
    if(!model->select())
        qDebug() << model->lastError().text();


    proxy = new QSortFilterProxyModel(this);
    proxy->setSourceModel(model);
    proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
    proxy->setFilterKeyColumn(-1);

    rmodel = new QSqlRelationalTableModel(this);

    menu = new QMenu();
    menu->addAction("Связанные детали...", this, SLOT(openDetailLink()), Qt::Key_Enter);

    ui->tableView->setModel(proxy);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);

    ui->tableView->setSortingEnabled(true);
    ui->tableView->sortByColumn(1, Qt::AscendingOrder);

    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->verticalHeader()->hide();
    ui->tableView->hideColumn(0);

    settings = Settings::getInstance();
    this->restoreGeometry(settings->value("detailList/geometry").toByteArray());
}

DetailList::~DetailList()
{
    settings->setValue("detailList/geometry", this->saveGeometry());
    delete ui;
}

void DetailList::on_tableView_customContextMenuRequested(const QPoint &pos)
{
    // получаем глобальные координаты
    QPoint globalPos;
    if (sender()->inherits("QAbstractScrollArea"))
        globalPos = ((QAbstractScrollArea*)sender())->viewport()->mapToGlobal(pos);
    else
        globalPos = ((QWidget*)sender())->mapToGlobal(pos);

    menu->exec(globalPos);
}

void DetailList::openDetailLink()
{
    QModelIndexList index = ui->tableView->selectionModel()->selectedRows(0);
    if(index.isEmpty())
    {
        qDebug() << tr("Ничего не выделенно");
        return;
    }

    selectRow = ui->tableView->currentIndex().row();
    QString id = index.at(0).data().toString();

    rmodel->setTable("carenlinkeddetailnames");
    rmodel->setFilter("ID_GroupDetail=" + id);
    rmodel->setRelation(0, QSqlRelation("carendetailnames", "ID", "Name"));
    rmodel->setRelation(1, QSqlRelation("carendetailnames", "ID", "Name"));

    if(!rmodel->select())
    {
        qDebug() << rmodel->lastError().text();
        return;
    }

    inLinked = true;
    proxy->setSourceModel(rmodel);
    ui->tableView->showColumn(0);
}

void DetailList::on_pushButton_clicked()
{
    proxy->setFilterFixedString(ui->lineEdit->text());
}

void DetailList::backToDetail()
{
    inLinked = false;
    proxy->setSourceModel(model);
    ui->tableView->hideColumn(0);
    ui->tableView->selectRow(selectRow);
}

void DetailList::addDetail()
{
    if(!inLinked)
    {
        bool ok;
        QString text = QInputDialog::getText(this, tr("Введите наименование новой детали"),
                                             tr("Наименование: "), QLineEdit::Normal ,"", &ok);
        if(ok && !text.isEmpty())
        {
            int row = model->rowCount();
            if(!model->insertRow(row))
            {
                qDebug() << "Error! : " + model->lastError().text();
                return;
            }

            if(!model->setData(model->index(row, 1), text))
            {
                qDebug() << "Error! : " + model->lastError().text();
                model->revertAll();
                return;
            }

            if(!model->submitAll())
            {
                qDebug() << "Error!4 : " + model->lastError().text();
                model->revertAll();
                return;
            }
            ui->tableView->selectRow(0);
        }
    }
    else
    {
        bool ok;
        QStringList id;
        QStringList name;
        QSqlQuery query("SELECT ID, Name FROM carendetailnames ORDER BY Name ASC");
        while (query.next())
        {
            id   << query.value(0).toString();
            name << query.value(1).toString();
        }

        QString item = QInputDialog::getItem(this, tr("Выберите связанную деталь"), tr("Деталь"),
                                             &name, 0, false, &ok);
        if(ok && !item.isEmpty())
        {
            int i = 0;
            for(; i < name.count(); ++i)
            {
                if(item == name.at(i))
                    break;
            }

            //!!!
        }
    }
}

void DetailList::keyPressEvent(QKeyEvent *event)
{
    if((event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) && inLinked == false)
    {
        this->openDetailLink();
        QDialog::keyPressEvent(event);
        return;
    }
    if(event->key() == Qt::Key_Escape && inLinked == true)
    {
        this->backToDetail();
        return;
    }

    QDialog::keyPressEvent(event);
}
