#include "filterresult.h"
#include "ui_filterresult.h"

FilterResult::FilterResult(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FilterResult)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Window);
    this->setWindowTitle(tr("Результаты"));
    settings = Settings::getInstance();

    menu = new QMenu();
    menu->addAction("Подрбнее", this, SLOT(openFirm()), Qt::Key_Enter);

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);

    ui->tableView->setSortingEnabled(false);

    ui->tableView->horizontalHeader()->setStretchLastSection(true);

    this->restoreGeometry(settings->value("filterResult/geometry").toByteArray());
    connect(ui->tableView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(openFirm()));
}

FilterResult::~FilterResult()
{
    settings->setValue("filterResult/geometry", this->saveGeometry());
    settings->setValue("filterResult/column_0", ui->tableView->columnWidth(0));
    settings->setValue("filterResult/column_1", ui->tableView->columnWidth(1));
    settings->setValue("filterResult/column_2", ui->tableView->columnWidth(2));
    settings->setValue("filterResult/column_3", ui->tableView->columnWidth(3));
    settings->setValue("filterResult/column_4", ui->tableView->columnWidth(4));
    settings->setValue("filterResult/column_5", ui->tableView->columnWidth(5));
    settings->setValue("filterResult/column_6", ui->tableView->columnWidth(6));
    settings->setValue("filterResult/column_7", ui->tableView->columnWidth(7));
    settings->setValue("filterResult/column_8", ui->tableView->columnWidth(8));
    settings->setValue("filterResult/column_9", ui->tableView->columnWidth(9));
    delete ui;
}

void FilterResult::setModel(QAbstractItemModel *model)
{
    proxy = new FilterResulpProxyModel(this);
    proxy->setSourceModel(model);
    ui->tableView->setModel(proxy);
    ui->tableView->setColumnWidth(0, settings->value("filterResult/column_0", 100).toInt());
    ui->tableView->setColumnWidth(1, settings->value("filterResult/column_1", 100).toInt());
    ui->tableView->setColumnWidth(2, settings->value("filterResult/column_2", 100).toInt());
    ui->tableView->setColumnWidth(3, settings->value("filterResult/column_3", 100).toInt());
    ui->tableView->setColumnWidth(4, settings->value("filterResult/column_4", 100).toInt());
    ui->tableView->setColumnWidth(5, settings->value("filterResult/column_5", 100).toInt());
    ui->tableView->setColumnWidth(6, settings->value("filterResult/column_6", 100).toInt());
    ui->tableView->setColumnWidth(7, settings->value("filterResult/column_7", 100).toInt());
    ui->tableView->setColumnWidth(8, settings->value("filterResult/column_8", 100).toInt());
    ui->tableView->setColumnWidth(9, settings->value("filterResult/column_9", 100).toInt());
    ui->tableView->resizeRowsToContents();
    ui->tableView->selectRow(0);
    connect(ui->tableView->horizontalHeader(), SIGNAL(sectionResized(int,int,int)),
            ui->tableView, SLOT(resizeRowsToContents()));
}

void FilterResult::on_tableView_customContextMenuRequested(const QPoint &pos)
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


void FilterResult::openFirm()
{
    QModelIndexList index = ui->tableView->selectionModel()->selectedIndexes();
    if(index.isEmpty())
    {
        qDebug() << index.first();
        return;
    }

    QSqlTableModel *firm = new QSqlTableModel(this);
    firm->setTable("firms");
    firm->setFilter("ID=" + index.at(0).data().toString());
    if(!firm->select()) {
        qDebug() << "Ошибка! Не могу найти фирму: " + firm->lastError().text();
    }

    FirmDialog *fd = new FirmDialog(this);
    fd->setModel(firm);
    fd->setCurrent(index.first());
    fd->setReadOnly(true);
    fd->exec();
    delete fd;
    delete firm;
}

void FilterResult::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
        this->openFirm();
    else
        QDialog::keyPressEvent(event);
}
