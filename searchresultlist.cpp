#include "searchresultlist.h"
#include "ui_searchresultlist.h"

SearchResultList::SearchResultList(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SearchResultList)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Window);
    this->setWindowTitle(tr("Результаты поиска"));
    model = new QSqlTableModel(this);
    proxy = new QSortFilterProxyModel(this);
    proxy->setSourceModel(model);
    settings = Settings::getInstance();

    menu = new QMenu();
    menu->addAction("Подрбнее", this, SLOT(openFirm()), Qt::Key_Enter);

    this->restoreGeometry(settings->value("searchDialog/geometry").toByteArray());
    connect(ui->tableView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(openFirm()));
}

SearchResultList::~SearchResultList()
{
    settings->setValue("searchDialog/geometry", this->saveGeometry());
    delete ui;
}

void SearchResultList::setSearch(QString text, QString append)
{
    model->setTable("firms");
    QString where = "(Name LIKE '%" + text + "%' OR "
                    "Address LIKE '%" + text + "%' OR "
                    "Phone LIKE '%" + text + "%' OR "
                    "Comment LIKE '%" + text + "%' OR "
                    "ActivityType LIKE '%" + text + "%' OR "
                    "OrganizationType LIKE '%" + text + "%' OR "
                    "District LIKE '%" + text + "%' OR "
                    "Fax LIKE '%" + text + "%' OR "
                    "Email LIKE '%" + text + "%' OR "
                    "URL LIKE '%" + text + "%' OR "
                    "OperatingMode LIKE '%" + text + "%')";

    if(!append.isEmpty()) {
        QStringList tmp = append.split(" ");
        for (int i = 0; i < tmp.count(); ++i) {
            where += "AND (Name LIKE '%" + tmp.at(i) + "%' OR "
                          "Address LIKE '%" + tmp.at(i) + "%' OR "
                          "Phone LIKE '%" + tmp.at(i) + "%' OR "
                          "Comment LIKE '%" + tmp.at(i) + "%' OR "
                          "ActivityType LIKE '%" + tmp.at(i) + "%' OR "
                          "OrganizationType LIKE '%" + tmp.at(i) + "%' OR "
                          "District LIKE '%" + tmp.at(i) + "%' OR "
                          "Fax LIKE '%" + tmp.at(i) + "%' OR "
                          "Email LIKE '%" + tmp.at(i) + "%' OR "
                          "URL LIKE '%" + tmp.at(i) + "%' OR "
                          "OperatingMode LIKE '%" + tmp.at(i) + "%')";
        }
    }

    model->setFilter(where);
    if(!model->select())
    {
        qDebug() << "Error!!!!! :" + model->lastError().text();
        return;
    }

    ui->tableView->verticalHeader()->hide();
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);

    ui->tableView->setSortingEnabled(true);
    ui->tableView->sortByColumn(1, Qt::AscendingOrder);

    ui->tableView->setModel(proxy);

    ui->tableView->resizeColumnsToContents();
    ui->tableView->resizeRowsToContents();
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->hideColumn(0);
    ui->tableView->hideColumn(4);
    ui->tableView->hideColumn(5);
    ui->tableView->hideColumn(6);
    ui->tableView->hideColumn(7);
    ui->tableView->hideColumn(8);
    ui->tableView->hideColumn(9);
    ui->tableView->hideColumn(10);
    ui->tableView->hideColumn(11);
    ui->tableView->hideColumn(12);
    ui->tableView->hideColumn(13);
    ui->tableView->hideColumn(14);

    ui->label->setText("Найдено " + QString::number(model->rowCount()) + " фирм");
    ui->tableView->selectRow(0);
    ui->tableView->setFocus();
}

void SearchResultList::on_tableView_customContextMenuRequested(const QPoint &pos)
{
    // получаем глобальные координаты
    QPoint globalPos;
    // если запрос от QAbstractScrollArea
    if (sender()->inherits("QAbstractScrollArea"))
        globalPos = ((QAbstractScrollArea*)sender())->viewport()->mapToGlobal(pos);
    // если от других виджетов
    else
        globalPos = ((QWidget*)sender())->mapToGlobal(pos);

    menu->exec(globalPos);
}

void SearchResultList::openFirm()
{
    QModelIndexList index = ui->tableView->selectionModel()->selectedIndexes();
    if(index.isEmpty())
    {
        qDebug() << index.first();
        return;
    }

    FirmDialog *fd = new FirmDialog(this);
    fd->setModel(proxy);
    fd->setCurrent(index.first());
    fd->setReadOnly(true);
    fd->exec();
    delete fd;
}

void SearchResultList::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
        this->openFirm();
    else
        QDialog::keyPressEvent(event);
}
