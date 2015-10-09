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
            ui->tableView->setModel(model);
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
