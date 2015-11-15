#include "basewizarddialog.h"
#include "ui_basewizarddialog.h"

BaseWizardDialog::BaseWizardDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BaseWizardDialog)
{
    ui->setupUi(this);
    ui->textBrowser->hide();
    settings = Settings::getInstance();
    this->setWindowTitle("Менеджер базы данных");
    baseType = settings->value("base/type").toString();
    if(baseType == LOCAL)
    {
        ui->tableView->setEnabled(false);
        ui->tableView->hide();
        ui->creatButton->setEnabled(false);
        ui->deleteButton->setEnabled(false);
        ui->label->setText("РАБОТА С ЛОКАЛЬНОЙ БАЗОЙ");
    }
    else
    {
        if(baseType == SERVER)
        {
            model = new QSqlQueryModel(this);
            this->updateBaseList();
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
}

QString BaseWizardDialog::getSelectBase()
{
    QModelIndexList index = ui->tableView->selectionModel()->selectedIndexes();
    if(!index.isEmpty())
        return index[0].data().toString();
    else
        return "";
}

void BaseWizardDialog::on_creatButton_clicked()
{
    this->setDisabled(true);
    bool ok;
    QString name = QInputDialog::getText(this, tr("Создание базы данных"),
                                         tr("Имя базы данных (без пробелов):"),
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
        this->updateBaseList();
    }
    this->setDisabled(false);
}

void BaseWizardDialog::on_deleteButton_clicked()
{
    QString selectbase = this->getSelectBase();
    if(selectbase == "")
    {
        QMessageBox msgBox;
        msgBox.setText("Выберите базу!");
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setInformativeText("Чтобы удалить базу выберите её из списка слева");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
        return;
    }

    if(selectbase == "mysql" || selectbase == "performance_schema" || selectbase == "information_schema")
    {
        QMessageBox msgBox;
        msgBox.setText("Нельза выбрать эту базу!");
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setInformativeText("Действи на данной базой запрещены. Она служебканая.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
        return;
    }

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
        query.prepare("drop database " + selectbase);
        if(!query.exec())
        {
            QString err = query.lastError().text();
            qDebug() << err;
            QMessageBox::critical(0, qApp->tr("Не могу удалить базу!"), err , QMessageBox::Ok);
            return;
        }
        else
        {
            this->updateBaseList();
        }
    }
}

void importToBase(QString basename, QString path, QTextBrowser *logger, QProgressBar *bar, QString basetype)
{
    BaseImport *import = new BaseImport;
    QObject::connect(import, SIGNAL(messages(QString)), logger, SLOT(append(QString)));
    QObject::connect(import, SIGNAL(fail(QString)), logger, SLOT(append(QString)));
    QObject::connect(import, SIGNAL(setMaxBar(int)), bar, SLOT(setMaximum(int)));
    QObject::connect(import, SIGNAL(setValueToBar(int)), bar, SLOT(setValue(int)));
    import->setBaseType(basetype);
    import->setPathToFiles(path);
    import->startImport(basename);
}

void BaseWizardDialog::on_importButton_clicked()
{
    QString selectbase = "";

    if(baseType == SERVER)
    {
        selectbase = this->getSelectBase();
        if(selectbase == "")
        {
            QMessageBox msgBox;
            msgBox.setText("Выберите базу!");
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setInformativeText("Чтобы импортировать данные в базу сначала выберите её из списка слева.");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.exec();
            return;
        }

        if(selectbase == "mysql" || selectbase == "performance_schema" || selectbase == "information_schema")
        {
            QMessageBox msgBox;
            msgBox.setText("Нельза выбрать эту базу!");
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setInformativeText("Действи на данной базой запрещены. Она служебная.");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.exec();
            return;
        }
    }

    ui->textBrowser->show();

    QDialog *parent = qobject_cast<QDialog *>(this->parent());
    QFileDialog dialog(parent);
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setNameFilter("Directories");
    dialog.setViewMode(QFileDialog::Detail);

    QString path;
    if (dialog.exec())
        path = dialog.selectedFiles().isEmpty() ? QString() : dialog.selectedFiles()[0];
    extern void importToBase(QString basename, QString path, QTextBrowser *logger, QProgressBar *bar, QString baseType_);
    QFuture<void> future = QtConcurrent::run(importToBase, selectbase, path, ui->textBrowser, ui->progressBar, baseType);

}


void BaseWizardDialog::updateBaseList()
{
    model->setQuery("show databases");
}

void BaseWizardDialog::disabledUi()
{
    ui->tableView->setEnabled(false);
    ui->creatButton->setEnabled(false);
    ui->deleteButton->setEnabled(false);
    ui->exportButton->setEnabled(false);
    ui->importButton->setEnabled(false);
}

void BaseWizardDialog::enabledUi()
{
    ui->tableView->setEnabled(true);
    ui->creatButton->setEnabled(true);
    ui->deleteButton->setEnabled(true);
    ui->exportButton->setEnabled(true);
    ui->importButton->setEnabled(true);
}

void BaseWizardDialog::setMsgToLog(QString msg)
{
    ui->textBrowser->append(msg);
}
