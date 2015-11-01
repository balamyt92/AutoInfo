#include "serviceeditdialog.h"
#include "ui_serviceeditdialog.h"

ServiceEditDialog::ServiceEditDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ServiceEditDialog)
{
    ui->setupUi(this);
}

ServiceEditDialog::~ServiceEditDialog()
{
    delete ui;
}
