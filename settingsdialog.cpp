#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Настройки");
    settings = Settings::getInstance();
    baseType = settings->value("base/type", SERVER).toString();
    if(baseType == SERVER)
    {
        ui->comboBox->setCurrentIndex(0);
        ui->hostEdit->setText(settings->value("server/hostname", "localhost").toString());
        ui->userEdit->setText(settings->value("server/username", "root").toString());
        ui->passwordEdit->setText(settings->value("server/password", "data").toString());
        ui->portEdit->setText(settings->value("server/port","3306").toString());
        ui->groupBox_2->setEnabled(false);
    }
    if(baseType == LOCAL)
    {
        ui->comboBox->setCurrentIndex(1);
        ui->groupBox->setEnabled(false);
        ui->pathEdit->setText(settings->value("localbase/basePath", "base.sqlite").toString());
    }
}

SettingsDialog::~SettingsDialog()
{
    settings->sync();
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
        settings->setValue("base/type", LOCAL);
        settings->setValue("localbase/basePath", ui->pathEdit->text());
    }
    settings->sync();
}

void SettingsDialog::on_comboBox_currentIndexChanged(int index)
{
    if(index)
    {
        ui->groupBox->setEnabled(false);
        ui->groupBox_2->setEnabled(true);
    }
    else
    {
        ui->groupBox->setEnabled(true);
        ui->groupBox_2->setEnabled(false);
        ui->hostEdit->setText(settings->value("server/hostname", "localhost").toString());
        ui->userEdit->setText(settings->value("server/username", "root").toString());
        ui->passwordEdit->setText(settings->value("server/password", "data").toString());
        ui->portEdit->setText(settings->value("server/port","3306").toString());
    }
}

#include <QFileDialog>
void SettingsDialog::on_pathButton_clicked()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setNameFilter("base (*.sqlite *.db)");
    dialog.setViewMode(QFileDialog::Detail);

    QString path;
    if (dialog.exec())
        path = dialog.selectedFiles().isEmpty() ? QString() : dialog.selectedFiles()[0];

    ui->pathEdit->setText(path);
}
