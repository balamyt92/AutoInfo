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
    model->setSort(1, Qt::AscendingOrder);
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

    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->setTabKeyNavigation(false);

    settings = Settings::getInstance();
    this->restoreGeometry(settings->value("firmList/geometry").toByteArray());

    ui->tableView->selectRow(0);
    ui->tableView->setFocus();

    ui->countLable->setText(QString::number(model->rowCount()));

    connect(ui->tableView->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(enableSort()));


    ui->tableView->setColumnWidth(0, settings->value("firmList/column_0", 100).toInt());
    ui->tableView->setColumnWidth(1, settings->value("firmList/column_1", 100).toInt());
    ui->tableView->setColumnWidth(2, settings->value("firmList/column_2", 100).toInt());
}

FirmsList::~FirmsList()
{
    settings->setValue("firmList/geometry", this->saveGeometry());
    settings->setValue("firmList/column_0", ui->tableView->columnWidth(0));
    settings->setValue("firmList/column_1", ui->tableView->columnWidth(1));
    settings->setValue("firmList/column_2", ui->tableView->columnWidth(2));
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

    PriceList *price = new PriceList(this);
    if(!price->setFirmID(index.first().data().toString())) {
        QMessageBox::warning(this, tr("Ошибка"), tr("Не могу открыть прайс!"));
    }
    price->exec();
    delete price;
}

void FirmsList::openServices()
{
    QModelIndexList index = ui->tableView->selectionModel()->selectedRows();
    if(index.isEmpty())
        return;

    PriceList *price = new PriceList(this);
    if(!price->openService(index.first().data().toString())) {
        QMessageBox::warning(this, tr("Ошибка"), tr("Не могу открыть прайс!"));
    }
    price->exec();
    delete price;
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

void FirmsList::enableSort()
{
    ui->tableView->setSortingEnabled(true);
    disconnect(ui->tableView->horizontalHeader(), SIGNAL(sectionClicked(int)), this, 0);
}
