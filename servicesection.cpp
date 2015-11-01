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
    menu->addAction("Изменить...", this, SLOT(editSection()), Qt::CTRL + Qt::Key_U);
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
        ui->tableView->selectRow(0);
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
        int row = ui->tableView->currentIndex().row();
        if(!proxy->removeRow(row))
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
        if(row > 1)
            ui->tableView->selectRow(row - 1);
        else
            ui->tableView->selectRow(row);
    }


}

void ServiceSection::openSection()
{
    QModelIndexList index = ui->tableView->selectionModel()->selectedRows();
    if(index.isEmpty())
        return;
    if(!sectionIsOpen)
    {

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

#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QGridLayout>
#include <QDialogButtonBox>
#include <QSqlQuery>

void ServiceSection::editSection()
{
    QModelIndexList index = ui->tableView->selectionModel()->selectedIndexes();
    if(index.isEmpty())
        return;
    if(sectionIsOpen)
    {

        QSqlQuery query("SELECT ID, Name FROM services "
                        "WHERE ID_Parent IS NULL "
                        "ORDER BY Name ASC ");
        QStringList id;
        QStringList name;
        while (query.next())
        {
            id << query.value(0).toString();
            name << query.value(1).toString();
        }

        QDialog *edit = new QDialog(this);
        edit->setWindowTitle("Редактирование услуги");
        QLabel *sectionName = new QLabel(tr("Раздел"), this);
        QLabel *nameLable = new QLabel(tr("Наименование"), edit);
        QLineEdit *line = new QLineEdit(edit);
        QComboBox *box = new QComboBox(edit);
        QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Ok |
                                                         QDialogButtonBox::Cancel, edit);
        connect(buttons, SIGNAL(accepted()), edit, SLOT(accept()));
        connect(buttons, SIGNAL(rejected()), edit, SLOT(reject()));

        QGridLayout *grid = new QGridLayout(edit);
        grid->addWidget(nameLable, 0, 0, 1, 1);
        grid->addWidget(sectionName, 1, 0, 1, 1);
        grid->addWidget(line, 0, 1, 1, 1);
        grid->addWidget(box, 1, 1, 1, 1);
        grid->addWidget(buttons, 2, 1, 1, 1);
        edit->setLayout(grid);

        line->setText(index.at(1).data().toString());
        box->addItems(name);
        int i = 0;
        for(; i < id.count(); ++i)
        {
            if(id.at(i).toInt() == index.at(3).data().toInt())
                break;
        }
        box->setCurrentIndex(i);

        if(edit->exec() == QDialog::Accepted)
        {
            query.prepare("UPDATE services SET ID_Parent="
                          + id.at(box->currentIndex()) +
                          ", Name=\'" + line->text() + "\'"
                          " WHERE ID=" + index.at(0).data().toString());
            query.exec();
            model->select();
            ui->tableView->setFocus();
        }

    }
    else
    {
        START:
        bool ok = false;
        QString newName = QInputDialog::getText(this, "Введите новое имя раздела",
                                                "Новое имя", QLineEdit::Normal,
                                                index.at(1).data().toString(), &ok);
        if(ok)
        {
            if(newName.isEmpty())
            {
                QMessageBox::critical(this, "Ошибка", "Имя не может быть пустым", QMessageBox::Ok);
                goto START;
            }

            if(!proxy->setData(index.at(1),newName))
            {
                QMessageBox::critical(this, "Ошибка", "Недопустимое имя", QMessageBox::Ok);
                goto START;
            }
            if(!model->submitAll())
            {
                QMessageBox::critical(this, "Ошибка", "Недопустимое имя", QMessageBox::Ok);
                goto START;
            }
        }
    }
}

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
