#include "servicesection.h"
#include "ui_servicesection.h"

ServiceSection::ServiceSection(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ServiceSection)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Window);
    this->setWindowTitle(tr("Услуги"));
    model = new QSqlTableModel(this);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->setTable("services");
    model->setFilter("ID_Parent IS NULL");
    if(!model->select())
    {
        qDebug() << "Error : " + model->lastError().text();
        return;
    }

    sectionIsOpen = false;

    proxy = new QSortFilterProxyModel(this);
    proxy->setSourceModel(model);

    menu = new QMenu();
    menu->addAction("Содержание...", this, SLOT(openSection()), Qt::Key_Enter);
    menu->addAction("Добавить...", this, SLOT(addSection()), Qt::CTRL + Qt::Key_A);
    menu->addAction("Удалить...", this, SLOT(deleteSection()), Qt::CTRL + Qt::Key_D);

    settings = Settings::getInstance();
    this->restoreGeometry(settings->value("serviceSectionDialog/geometry").toByteArray());



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
    ui->tableView->hideColumn(0);
    ui->tableView->hideColumn(2);
    ui->tableView->hideColumn(3);

    ui->tableView->selectRow(0);
    ui->tableView->setFocus();
}

ServiceSection::~ServiceSection()
{
    settings->setValue("serviceSectionDialog/geometry", this->saveGeometry());
    delete ui;
}

#include <QInputDialog>
void ServiceSection::addSection()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Введите название новго раздела"),
                                         tr("Наименование: "), QLineEdit::Normal ,"", &ok);
    if(ok && !text.isEmpty())
    {
        int row = model->rowCount();
        if(!model->insertRow(row))
        {
            qDebug() << "Error!1 : " + model->lastError().text();
            return;
        }

        if(sectionIsOpen)
        {
            if(!model->setData(model->index(row, 3), id))
            {
                qDebug() << "Error!2 : " + model->lastError().text();
                model->revertAll();
                return;
            }
        }

        if(!model->setData(model->index(row, 1), text))
        {
            qDebug() << "Error!3 : " + model->lastError().text();
            model->revertAll();
            return;
        }

        if(!model->submitAll())
        {
            qDebug() << "Error!4 : " + model->lastError().text();
            model->revertAll();
            return;
        }
    }
}

void ServiceSection::deleteSection()
{
    QMessageBox msg;
    msg.setIcon(QMessageBox::Warning);

    if(!sectionIsOpen)
    {
        msg.setText("Вы хотите удалить раздел " + ui->tableView->currentIndex().data().toString());
    }
    else
    {
        msg.setText("Вы хотите удалить услугу " + ui->tableView->currentIndex().data().toString());
    }

    msg.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msg.setDefaultButton(QMessageBox::Cancel);

    int ret = msg.exec();

    if(ret == QMessageBox::Ok)
    {
        if(!proxy->removeRow(ui->tableView->currentIndex().row()))
        {
            qDebug() << "Error! : " + model->lastError().text();
            qDebug() << "----------------------------------------";
            model->revertAll();

            msg.setIcon(QMessageBox::Critical);
            msg.setText("Не могу удалить!");
            msg.setInformativeText(model->lastError().text());
            msg.setStandardButtons(QMessageBox::Ok);
            msg.exec();
            return;
        }
        if(!model->submitAll())
        {
            qDebug() << "Error! : " + model->lastError().text();
            qDebug() << "----------------------------------------";
            model->revertAll();

            msg.setIcon(QMessageBox::Critical);
            msg.setText("Не могу удалить!");
            msg.setInformativeText(model->lastError().text());
            msg.setStandardButtons(QMessageBox::Ok);
            msg.exec();
            return;
        }
    }


}

void ServiceSection::openSection()
{
    if(!sectionIsOpen)
    {
        QModelIndexList index = ui->tableView->selectionModel()->selectedRows();
        selectedSection = ui->tableView->currentIndex().row();
        id = index.first().data().toString();
        model->setFilter("ID_Parent=" + id);
        if(!model->select())
        {
            qDebug() << "Error : " + model->lastError().text();
        }
        sectionIsOpen = true;
        ui->tableView->selectRow(0);
        ui->tableView->setFocus();
    }
    else
    {
        QModelIndexList index = ui->tableView->selectionModel()->selectedRows();
        ServiceSearchResult *sr = new ServiceSearchResult(this);
        sr->setServiceId(index.at(0).data().toInt());
        sr->exec();
        delete sr;
    }
}

void ServiceSection::on_tableView_customContextMenuRequested(const QPoint &pos)
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

void ServiceSection::backToSections()
{
    model->setFilter("ID_Parent IS NULL");
    if(!model->select())
    {
        qDebug() << "Error : " + model->lastError().text();
        return;
    }
    sectionIsOpen = false;
    ui->tableView->selectRow(selectedSection);
    ui->tableView->setFocus();
}

#include <QKeyEvent>
void ServiceSection::keyPressEvent(QKeyEvent *event)
{
    if(sectionIsOpen == true && event->key() == Qt::Key_Escape)
    {
        this->backToSections();
        return;
    }
    if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
    {
        this->openSection();
        return;
    }

    QDialog::keyPressEvent(event);
}
