#include "basewizarddialog.h"
#include "ui_basewizarddialog.h"

BaseWizardDialog::BaseWizardDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BaseWizardDialog)
{
    ui->setupUi(this);
    settings = new QSettings(CONFIG_FILE, QSettings::IniFormat);
    this->setWindowTitle("Менеджер базы данных");
    baseType = settings->value("base/type").toString();
    if(baseType == LOCAL)
    {
        ui->tableView->setEnabled(false);
        ui->creatButton->setEnabled(false);
        ui->deleteButton->setEnabled(false);
        ui->label->setText("РАБОТА С ЛОКАЛЬНОЙ БАЗОЙ");
    }
    else
    {
        if(baseType == SERVER)
        {
            model = new QSqlQueryModel(this);
            model->setQuery("show databases");
            proxyModel = new QSortFilterProxyModel(this);
            proxyModel->setSourceModel(model);
            ui->tableView->setModel(proxyModel);
            ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
            ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
            ui->tableView->resizeColumnsToContents();
            ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
            ui->tableView->setSortingEnabled(true);
            ui->tableView->horizontalHeader()->setStretchLastSection(true);
            ui->tableView->verticalHeader()->hide();

        }
        else
        {
            qDebug() << "Не могу определить тип базы!";
            QMessageBox msgBox;
            msgBox.setText("Не удалось определить тип базы данных!");
            msgBox.setInformativeText("Неправильно указан тип базы данных в конфиге base/type");
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.exec();
            this->close();
        }
    }

}

BaseWizardDialog::~BaseWizardDialog()
{
    delete ui;
    settings->setValue("MainWindows/geometry", this->saveGeometry());
    settings->sync();
}

QString BaseWizardDialog::getSelectBase()
{
    QModelIndexList index = ui->tableView->selectionModel()->selectedIndexes();
    if(index[0].isValid())
        return index[0].data().toString();
    else
        return "";
}

void BaseWizardDialog::on_creatButton_clicked()
{
    bool ok;
    QString name = QInputDialog::getText(this, tr("Создание базы данных"),
                                         tr("Имя мазы данных (без пробелов):"),
                                         QLineEdit::Normal, "name", &ok);
    if (ok && !name.isEmpty())
    {
        QSqlQuery add;
        add.prepare("create database " + name);
        if (!add.exec())
        {
            QString err = add.lastError().text();
            qDebug() << err;
            QMessageBox::critical(0, qApp->tr("Не могу добавить базу!"), err , QMessageBox::Ok);
            return;
        }
        else
        {
            QFile file(":/files/create_base.sql");
            if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                QString err = file.errorString();
                qDebug() << err;
                QMessageBox::critical(0, qApp->tr("Не могу добавить базу!"), err , QMessageBox::Cancel);
                return;
            }

            QTextStream in(&file);
            in.setCodec("UTF-8");
            QString sql = in.readAll();
            sql.replace(QString("`auto`"), "`" + name + "`");

            QStringList sqlList = sql.split(";");
            sqlList.removeLast();

            ui->progressBar->setMaximum(sqlList.count());

            while (!sqlList.empty())
            {
                QString query = sqlList.takeFirst();
                add.prepare(query);
                qDebug() << query;
                if (!add.exec())
                {
                    QString err = add.lastError().text();
                    qDebug() << err;
                    QMessageBox::critical(0, qApp->tr("Не могу добавить базу!"), err , QMessageBox::Cancel);
                    ui->progressBar->setValue(0);
                    return;
                }
                qDebug() << "true!!!!!";
                ui->progressBar->setValue(ui->progressBar->value() + 1);
            }

            add.prepare("INSERT INTO `" + name + "`.`marktypes` (`ID`, `Name`) VALUES ('1', 'марка')");
            if (!add.exec())
            {
                QString err = add.lastError().text();
                qDebug() << err;
                QMessageBox::critical(0, qApp->tr("Не могу добавить значения базу!"), err , QMessageBox::Cancel);
                return;
            }

            add.prepare("INSERT INTO `" + name + "`.`marktypes` (`ID`, `Name`) VALUES ('2', 'группа')");
            if (!add.exec())
            {
                QString err = add.lastError().text();
                qDebug() << err;
                QMessageBox::critical(0, qApp->tr("Не могу добавить значения базу!"), err , QMessageBox::Cancel);
                return;
            }

            add.prepare("INSERT INTO `" + name + "`.`modeltypes` (`ID`, `Name`) VALUES ('1', 'модель')");
            if (!add.exec())
            {
                QString err = add.lastError().text();
                qDebug() << err;
                QMessageBox::critical(0, qApp->tr("Не могу добавить значения базу!"), err , QMessageBox::Cancel);
                return;
            }

            add.prepare("INSERT INTO `" + name + "`.`modeltypes` (`ID`, `Name`) VALUES ('2', 'группа')");
            if (!add.exec())
            {
                QString err = add.lastError().text();
                qDebug() << err;
                QMessageBox::critical(0, qApp->tr("Не могу добавить значения базу!"), err , QMessageBox::Cancel);
                return;
            }
            ui->progressBar->setValue(0);
        }
    }
    model->setQuery("show databases");
}

void BaseWizardDialog::on_deleteButton_clicked()
{
    QMessageBox msgBox;
    msgBox.setText("<b>Удаление базы!</b>");
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setInformativeText("Вы действительно хотите удалить базу " + this->getSelectBase() + "?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    int ret = msgBox.exec();
    if (ret == QMessageBox::Ok)
    {
        QSqlQuery query;
        query.prepare("drop database " + this->getSelectBase());
        if(!query.exec())
        {
            QString err = query.lastError().text();
            qDebug() << err;
            QMessageBox::critical(0, qApp->tr("Не могу удалить базу!"), err , QMessageBox::Ok);
            return;
        }
        else
        {
            model->setQuery("show databases");
        }
    }
}
