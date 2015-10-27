#include "searchresultlist.h"
#include "ui_searchresultlist.h"

SearchResultList::SearchResultList(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SearchResultList)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Window);
    model = new QSqlTableModel(this);
    proxy = new QSortFilterProxyModel(this);
    proxy->setSourceModel(model);
    settings = Settings::getInstance();

    menu = new QMenu();
    menu->addAction("Подрбнее", this, SLOT(openFirm()), Qt::Key_Enter);

    this->restoreGeometry(settings->value("searchDialog/geometry").toByteArray());
}

SearchResultList::~SearchResultList()
{
    settings->setValue("searchDialog/geometry", this->saveGeometry());
    delete ui;
}

void SearchResultList::setSearch(QString text)
{
    model->setTable("firms");
    model->setFilter("concat(Name,Address,Phone,Comment,ActivityType,OrganizationType,District,Fax,Email,URL,OperatingMode) LIKE '%" + text + "%'");
    if(!model->select())
    {
        qDebug() << model->lastError().text();
        return;
    }

    ui->tableView->setModel(proxy);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);

    ui->tableView->setSortingEnabled(true);
    ui->tableView->sortByColumn(1, Qt::AscendingOrder);

    ui->tableView->resizeColumnsToContents();
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->verticalHeader()->hide();
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
