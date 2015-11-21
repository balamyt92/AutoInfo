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

    ui->tableView->setSortingEnabled(true);
    ui->tableView->sortByColumn(1, Qt::AscendingOrder);

    ui->tableView->horizontalHeader()->setStretchLastSection(true);

    this->restoreGeometry(settings->value("filterResult/geometry").toByteArray());
}

FilterResult::~FilterResult()
{
    settings->setValue("filterResult/geometry", this->saveGeometry());
    delete ui;
}

void FilterResult::setModel(QAbstractItemModel *model)
{
    ui->tableView->setModel(model);
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
