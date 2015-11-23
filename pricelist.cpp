#include "pricelist.h"
#include "ui_pricelist.h"

PriceList::PriceList(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PriceList)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Window);
    this->setWindowTitle(tr("Прайс"));

    model = new QSqlRelationalTableModel(this);
    //proxy = new QSortFilterProxyModel(this);

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->setSortingEnabled(false);

    ui->tableView->horizontalHeader()->setDragEnabled(true);
    ui->tableView->horizontalHeader()->setDragDropMode(QAbstractItemView::InternalMove);
    settings = Settings::getInstance();
    this->restoreGeometry(settings->value("priceList/geometry").toByteArray());

    connect(ui->tableView->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(enableSort(int)));
}

PriceList::~PriceList()
{
    settings->setValue("priceList/geometry", this->saveGeometry());
    settings->setValue("priceList/column_0", ui->tableView->columnWidth(0));
    settings->setValue("priceList/column_1", ui->tableView->columnWidth(1));
    settings->setValue("priceList/column_2", ui->tableView->columnWidth(2));
    settings->setValue("priceList/column_3", ui->tableView->columnWidth(3));
    settings->setValue("priceList/column_4", ui->tableView->columnWidth(4));
    settings->setValue("priceList/column_5", ui->tableView->columnWidth(5));
    settings->setValue("priceList/column_6", ui->tableView->columnWidth(6));
    settings->setValue("priceList/column_7", ui->tableView->columnWidth(7));
    settings->setValue("priceList/column_8", ui->tableView->columnWidth(8));
    settings->setValue("priceList/column_9", ui->tableView->columnWidth(9));
    settings->setValue("priceList/column_10", ui->tableView->columnWidth(10));
    delete ui;
}

bool PriceList::setFirmID(QString id)
{
    ID = id;
    model->setTable("carpresenceen");
    model->setFilter("ID_Firm=" + ID);
    model->setRelation(0, QSqlRelation("carmarksen", "ID", "Name"));
    model->setRelation(1, QSqlRelation("carmodelsen", "ID", "Name"));
    model->setRelation(2, QSqlRelation("carendetailnames", "ID", "Name"));
    model->setRelation(5, QSqlRelation("carbodymodelsen", "ID", "Name"));
    model->setRelation(6, QSqlRelation("carenginemodelsen", "ID", "Name"));
    model->setSort(1, Qt::AscendingOrder);
    if(!model->select()) {
        qDebug() << model->lastError().text();
        return false;
    }

    while (model->canFetchMore()) {
        model->fetchMore();
    }
    model->setHeaderData(0, Qt::Horizontal, tr("Марка"));
    model->setHeaderData(1, Qt::Horizontal, tr("Модель"));
    model->setHeaderData(2, Qt::Horizontal, tr("Деталь"));
    model->setHeaderData(3, Qt::Horizontal, tr("ID Фирмы"));
    model->setHeaderData(4, Qt::Horizontal, tr("Год"));
    model->setHeaderData(5, Qt::Horizontal, tr("Кузов"));
    model->setHeaderData(6, Qt::Horizontal, tr("Двигатель"));
    model->setHeaderData(7, Qt::Horizontal, tr("Коментарий"));
    model->setHeaderData(8, Qt::Horizontal, tr("Фото"));
    model->setHeaderData(9, Qt::Horizontal, tr("Номер"));
    model->setHeaderData(10, Qt::Horizontal, tr("Цена"));

    //proxy->setSourceModel(model);
    ui->tableView->setModel(model);
    ui->tableView->setSortingEnabled(true);

    ui->tableView->setColumnWidth(0, settings->value("priceList/column_0", 100).toInt());
    ui->tableView->setColumnWidth(1, settings->value("priceList/column_1", 100).toInt());
    ui->tableView->setColumnWidth(2, settings->value("priceList/column_2", 100).toInt());
    ui->tableView->setColumnWidth(3, settings->value("priceList/column_3", 100).toInt());
    ui->tableView->setColumnWidth(4, settings->value("priceList/column_4", 100).toInt());
    ui->tableView->setColumnWidth(5, settings->value("priceList/column_5", 100).toInt());
    ui->tableView->setColumnWidth(6, settings->value("priceList/column_6", 100).toInt());
    ui->tableView->setColumnWidth(7, settings->value("priceList/column_7", 100).toInt());
    ui->tableView->setColumnWidth(8, settings->value("priceList/column_8", 100).toInt());
    ui->tableView->setColumnWidth(9, settings->value("priceList/column_9", 100).toInt());
    ui->tableView->setColumnWidth(10, settings->value("priceList/column_10", 100).toInt());
    return true;
}

void PriceList::enableSort(int colum)
{
    model->setSort(colum, Qt::AscendingOrder);
    model->select();
    ui->tableView->horizontalHeader()->setSortIndicator(colum, Qt::AscendingOrder);
//    disconnect(ui->tableView->horizontalHeader(), SIGNAL(sectionClicked(int)), this, 0);
}
