#include "firmdialog.h"
#include "ui_firmdialog.h"

FirmDialog::FirmDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FirmDialog)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("Фирма"));
    okay = new QPushButton(tr("Ok"));
    canceled = new QPushButton(tr("Отмена"));
    okay->setAutoDefault(true);
    canceled->setAutoDefault(true);
    okay->setDefault(false);
    canceled->setDefault(true);
    ui->buttonBox->addButton(okay, QDialogButtonBox::AcceptRole);
    ui->buttonBox->addButton(canceled, QDialogButtonBox::RejectRole);
    canceled->setFocus();

    mapper = new QDataWidgetMapper;
    settings = Settings::getInstance();
    this->restoreGeometry(settings->value("firmDialog/geometry").toByteArray());
}

FirmDialog::~FirmDialog()
{
    settings->setValue("firmDialog/geometry", this->saveGeometry());
    delete ui;
}

void FirmDialog::setModel(QAbstractItemModel *model)
{
    mapper->setModel(model);
    mapper->addMapping(ui->lineID, 0);
    mapper->addMapping(ui->lineName, 1);
    mapper->addMapping(ui->lineAdres, 2);
    mapper->addMapping(ui->linePhone, 3);
    mapper->addMapping(ui->TextPrim, 4);
    mapper->addMapping(ui->comboBox, 5);
    mapper->addMapping(ui->TextProf, 6);
    mapper->addMapping(ui->lineOrgName, 7);
    mapper->addMapping(ui->lineRaion, 8);
    mapper->addMapping(ui->lineFax, 9);
    mapper->addMapping(ui->lineMail, 10);
    mapper->addMapping(ui->lineHTTP, 11);
    mapper->addMapping(ui->TextRegim, 12);
    mapper->addMapping(ui->lineIndifer, 13);
    mapper->addMapping(ui->linePriority, 14);
    mapper->toFirst();
}

void FirmDialog::setCurrent(QModelIndex &index)
{
    mapper->setCurrentModelIndex(index);
}

void FirmDialog::setReadOnly(bool flag)
{
    ui->lineID->setReadOnly(flag);
    ui->lineName->setReadOnly(flag);
    ui->lineAdres->setReadOnly(flag);
    ui->linePhone->setReadOnly(flag);
    ui->TextPrim->setReadOnly(flag);
    ui->comboBox->setDisabled(flag);
    ui->TextProf->setReadOnly(flag);
    ui->lineOrgName->setReadOnly(flag);
    ui->lineRaion->setReadOnly(flag);
    ui->lineFax->setReadOnly(flag);
    ui->lineMail->setReadOnly(flag);
    ui->lineHTTP->setReadOnly(flag);
    ui->TextRegim->setReadOnly(flag);
    ui->lineIndifer->setReadOnly(flag);
    ui->linePriority->setReadOnly(flag);
}



