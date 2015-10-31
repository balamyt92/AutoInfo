#include "servicesearchresult.h"
#include "ui_servicesearchresult.h"

ServiceSearchResult::ServiceSearchResult(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ServiceSearchResult)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Window);
    this->setWindowTitle(tr("Услуги"));
    model = new QSqlRelationalTableModel(this);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);

    proxy = new QSortFilterProxyModel(this);
    proxy->setSourceModel(model);

    menu = new QMenu();
    menu->addAction("О фирме...", this, SLOT(openFirm()), Qt::Key_Enter);

    settings = Settings::getInstance();
    this->restoreGeometry(settings->value("serviceSearchResultDialog/geometry").toByteArray());



    ui->tableView->setModel(proxy);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);

    ui->tableView->setSortingEnabled(true);
    ui->tableView->sortByColumn(1, Qt::AscendingOrder);

    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->verticalHeader()->hide();
}

ServiceSearchResult::~ServiceSearchResult()
{
    settings->setValue("serviceSearchResultDialog/geometry", this->saveGeometry());
    delete ui;
}

void ServiceSearchResult::setServiceId(int id_service)
{
    model->setJoinMode(QSqlRelationalTableModel::LeftJoin);

    model->setTable("servicepresence");
    model->setFilter("ID_Service=" + QString::number(id_service) + " AND Enabled=1");
    model->setRelation(1, QSqlRelation("firms", "ID", "ID, Name, Address, Phone, Enabled"));
    if(!model->select())
    {
        qDebug() << "Error! : " + model->lastError().text();
        qDebug() << "--------------------------------------";
    }

    ui->tableView->hideColumn(0);
    ui->tableView->hideColumn(1);
    ui->tableView->hideColumn(5);
    ui->tableView->selectRow(0);
    ui->tableView->setFocus();
    ui->tableView->resizeColumnsToContents();
}

void ServiceSearchResult::on_tableView_customContextMenuRequested(const QPoint &pos)
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

void ServiceSearchResult::openFirm()
{
    QModelIndexList index = ui->tableView->selectionModel()->selectedRows(1);
    QSqlTableModel *firm = new QSqlTableModel(this);
    firm->setTable("firms");
    firm->setFilter("ID=" + index.at(0).data().toString());
    if(!firm->select())
    {
        qDebug() << firm->lastError().text();
    }
    else
    {
        FirmDialog *fd = new FirmDialog(this);
        fd->setReadOnly(true);
        fd->setModel(firm);
        fd->exec();
        delete fd;
    }
    delete firm;
}

void ServiceSearchResult::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
    {
        this->openFirm();
        return;
    }

    QDialog::keyPressEvent(event);
}
