#include "firmslist.h"
#include "ui_firmslist.h"

FirmsList::FirmsList(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FirmsList)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Window);
    this->setWindowTitle(tr("Фирмы"));
    model = new QSqlTableModel(this);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->setTable("firms");
    if(!model->select())
    {
        qDebug() << "Error : " + model->lastError().text();
        return;
    }
    while (model->canFetchMore()) {
        model->fetchMore();
    }

    proxy = new FirmProxyModel(this);
    proxy->setSourceModel(model);
    proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
    proxy->setFilterKeyColumn(-1);
    proxy->setSortCaseSensitivity(Qt::CaseInsensitive);

    menu = new QMenu();
    menu->addAction("Прайс...", this, SLOT(openPrice()), Qt::Key_Enter);
    menu->addAction("Услуги...", this, SLOT(openServices()), Qt::CTRL + Qt::Key_Enter);
    menu->addAction("Изменить...", this, SLOT(editFirm()), Qt::CTRL + Qt::Key_U);
    menu->addAction("Добавить...", this, SLOT(addFirm()), Qt::CTRL + Qt::Key_A);
    menu->addAction("Удалить...", this, SLOT(deleteFirm()), Qt::CTRL + Qt::Key_D);


    ui->tableView->setModel(proxy);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);

    ui->tableView->setSortingEnabled(true);
    ui->tableView->sortByColumn(1, Qt::AscendingOrder);

    ui->tableView->resizeColumnsToContents();
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->verticalHeader()->hide();
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
    ui->tableView->hideColumn(15);


    settings = Settings::getInstance();
    this->restoreGeometry(settings->value("firmList/geometry").toByteArray());

    ui->tableView->selectRow(0);
    ui->tableView->setFocus();

    ui->countLable->setText(QString::number(model->rowCount()));
}

FirmsList::~FirmsList()
{
    settings->setValue("firmList/geometry", this->saveGeometry());
    delete ui;
}

void FirmsList::on_tableView_customContextMenuRequested(const QPoint &pos)
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

void FirmsList::on_filtrButton_clicked()
{
    proxy->setFilterFixedString(ui->lineEdit->text());
}

#include <QSqlRelationalTableModel>
#include <QSqlQuery>

void FirmsList::openPrice()
{
    QModelIndexList index = ui->tableView->selectionModel()->selectedRows();
    if(index.isEmpty())
        return;

    QDialog *d = new QDialog(this);
    d->setWindowFlags(Qt::Window);
    QTableView *v = new QTableView(d);
    v->setEditTriggers(QAbstractItemView::NoEditTriggers);
    QGridLayout *grid = new QGridLayout(d);
    grid->addWidget(v,0,0,0,0);
    d->setLayout(grid);

    QSqlRelationalTableModel *price = new QSqlRelationalTableModel(d);
    price->setTable("carpresenceen");
    price->setFilter("ID_Firm=" + index.first().data().toString());
    price->setRelation(0, QSqlRelation("carmarksen", "ID", "Name"));
    price->setRelation(1, QSqlRelation("carmodelsen", "ID", "Name"));
    price->setRelation(2, QSqlRelation("carendetailnames", "ID", "Name"));
    price->setRelation(5, QSqlRelation("carbodymodelsen", "ID", "Name"));
    price->setRelation(6, QSqlRelation("carenginemodelsen", "ID", "Name"));

    qDebug() << price->lastError().text();
    qDebug() << price->filter();

    price->select();

    while (price->canFetchMore()) {
        price->fetchMore();
    }

    v->setModel(price);
    d->resize(800, 800);
    d->exec();
    delete d;
}

void FirmsList::openServices()
{
    QModelIndexList index = ui->tableView->selectionModel()->selectedRows();
    if(index.isEmpty())
        return;

    QDialog *d = new QDialog(this);
    d->setWindowFlags(Qt::Window);
    QTableView *v = new QTableView(d);
    v->setEditTriggers(QAbstractItemView::NoEditTriggers);
    QGridLayout *grid = new QGridLayout(d);
    grid->addWidget(v,0,0,0,0);
    d->setLayout(grid);

    QSqlRelationalTableModel *price = new QSqlRelationalTableModel(d);
    price->setTable("servicepresence");
    price->setFilter("ID_Firm=" + index.first().data().toString());
    price->setRelation(0, QSqlRelation("services", "ID", "Name"));

    qDebug() << price->lastError().text();
    qDebug() << price->filter();

    price->select();

    while (price->canFetchMore()) {
        price->fetchMore();
    }

    v->setModel(price);
    d->resize(800, 800);
    d->exec();
    delete d;
}

void FirmsList::addFirm()
{

}

void FirmsList::deleteFirm()
{

}

#include "firmdialog.h"
void FirmsList::editFirm()
{
    QModelIndex index = ui->tableView->selectionModel()->selectedIndexes().first();
    if(!index.isValid())
        return;

    index = proxy->mapToSource(index);

    FirmDialog *fd = new FirmDialog(this);
    fd->setModel(model);
    fd->setCurrent(index);
    fd->exec();
    delete fd;

    model->submitAll();
    while (model->canFetchMore()) {
        model->fetchMore();
    }
}
