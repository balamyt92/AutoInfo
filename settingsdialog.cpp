#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Натсройки");
    settings = new QSettings(CONFIG_FILE, QSettings::IniFormat);
    baseType = settings->value("base/type", SERVER).toString();
    if(baseType == SERVER)
    {
        ui->comboBox->setCurrentIndex(0);
        ui->hostEdit->setText(settings->value("server/hostname", "localhost").toString());
        ui->userEdit->setText(settings->value("server/username", "root").toString());
        ui->passwordEdit->setText(settings->value("server/password", "data").toString());
        ui->portEdit->setText(settings->value("server/port","3306").toString());
    }
    if(baseType == LOCAL)
    {
        ui->comboBox->setCurrentIndex(1);
        ui->groupBox->setEnabled(false);
    }
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::on_buttonBox_accepted()
{
    if(ui->comboBox->currentIndex() == 0)
    {
        settings->setValue("server/hostname", ui->hostEdit->text());
        settings->setValue("base/type", SERVER);
        settings->setValue("server/username", ui->userEdit->text());
        settings->setValue("server/password", ui->passwordEdit->text());
        settings->setValue("server/port", ui->portEdit->text());
    }
    else
    {
        settings->setValue("localbase/hostname", "localhost");
        settings->setValue("localbase/basename", LOCAL_BASE_NAME);
        settings->setValue("base/type", LOCAL);
    }
    settings->sync();
}

void SettingsDialog::on_comboBox_currentIndexChanged(int index)
{
    if(index)
    {
        ui->groupBox->setEnabled(false);
    }
    else
    {
        ui->groupBox->setEnabled(true);
        ui->hostEdit->setText(settings->value("server/hostname", "localhost").toString());
        ui->userEdit->setText(settings->value("server/username", "root").toString());
        ui->passwordEdit->setText(settings->value("server/password", "data").toString());
        ui->portEdit->setText(settings->value("server/port","3306").toString());
    }
}
