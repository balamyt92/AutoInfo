#include "filterdialog.h"
#include "ui_filterdialog.h"

FilterDialog::FilterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FilterDialog)
{
    ui->setupUi(this);

    this->setWindowTitle("Поиск запчастей");
    ui->modelBox->setDisabled(true);
    ui->bodyBox->setDisabled(true);
    ui->engineBox->setDisabled(true);

    QSqlQuery query("SELECT ID, Name FROM carmarksen ORDER BY Name ASC");
    while (query.next())
    {
        id_mark << query.value(0).toString();
        name_mark << query.value(1).toString();
    }
    query.prepare("SELECT ID, Name FROM carendetailnames ORDER BY Name ASC");
    query.exec();
    qDebug() << query.lastError().text();
    while (query.next())
    {
        id_detail << query.value(0).toString();
        name_detail << query.value(1).toString();
    }

    ui->markBox->setFocus();
    ui->markBox->addItems(name_mark);
    ui->markBox->setCurrentIndex(-1);

    ui->detailBox->addItems(name_detail);
    ui->detailBox->setCurrentIndex(-1);

    ui->searchButton->setAutoDefault(false);

    connect(ui->markBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setModeles()));
}

FilterDialog::~FilterDialog()
{
    delete ui;
}

void FilterDialog::on_searchButton_clicked()
{

}

void FilterDialog::setModeles()
{

    bool flag = false;
    if(id_model.isEmpty())
    {
        ui->modelBox->setEnabled(true);
        flag = true;
    }
    else
    {
        id_model.clear();
        name_model.clear();
    }

    QSqlQuery query("SELECT ID, Name FROM carmodelsen WHERE ID_Mark="
                    + id_mark.at(ui->markBox->currentIndex()) +
                    " ORDER BY Name ASC");
    while(query.next())
    {
        id_model << query.value(0).toString();
        name_model << query.value(1).toString();
    }
    ui->modelBox->clear();
    ui->modelBox->addItems(name_model);
    ui->modelBox->setCurrentIndex(-1);

    if(flag)
    {
        connect(ui->modelBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setBodys()));
        connect(ui->bodyBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setEngines()));
    }
}

void FilterDialog::setBodys()
{
    if(!id_body.isEmpty())
    {
        id_body.clear();
        name_body.clear();
    }
    else
    {
        ui->bodyBox->setEnabled(true);
    }


    QSqlQuery query("SELECT ID, Name FROM carbodymodelsen WHERE ID_Mark="
                    + id_mark.at(ui->markBox->currentIndex()) + " AND "
                                                                "ID_Model=" + id_model.at(ui->modelBox->currentIndex()) +
                    " ORDER BY Name ASC");
    while(query.next())
    {
        id_body << query.value(0).toString();
        name_body << query.value(1).toString();
    }
    ui->bodyBox->clear();
    ui->bodyBox->addItems(name_body);
    ui->bodyBox->setCurrentIndex(-1);

}

void FilterDialog::setEngines()
{
    if(!id_engine.isEmpty())
    {
        id_engine.clear();
        name_engine.clear();
    }
    else
    {
        ui->engineBox->setEnabled(true);
    }

    QSqlQuery query;
    if(ui->bodyBox->currentIndex() >= 0)
    {
        query.prepare("SELECT carenginemodelsen.ID, carenginemodelsen.Name FROM carengineandbodycorrespondencesen"
                      " LEFT JOIN carenginemodelsen"
                      " ON carengineandbodycorrespondencesen.ID_Engine=carenginemodelsen.ID"
                      " WHERE carengineandbodycorrespondencesen.ID_Mark=" + id_mark.at(ui->markBox->currentIndex()) +
                      " AND carengineandbodycorrespondencesen.ID_Model=" + id_model.at(ui->modelBox->currentIndex()) +
                      " AND carengineandbodycorrespondencesen.ID_Body=" + id_body.at(ui->bodyBox->currentIndex()) +
                      " ORDER BY Name ASC");
    }
    else
    {
        query.prepare("SELECT carenginemodelsen.ID, carenginemodelsen.Name FROM carengineandmodelcorrespondencesen"
                      " LEFT JOIN carenginemodelsen"
                      " ON carengineandmodelcorrespondencesen.ID_Engine=carenginemodelsen.ID"
                      " WHERE carengineandmodelcorrespondencesen.ID_Mark=" + id_mark.at(ui->markBox->currentIndex()) +
                      " AND carengineandmodelcorrespondencesen.ID_Model=" + id_model.at(ui->modelBox->currentIndex()) +
                      " ORDER BY Name ASC");
    }
    query.exec();
    while(query.next())
    {
        id_engine << query.value(0).toString();
        name_engine << query.value(1).toString();
    }
    ui->engineBox->clear();
    ui->engineBox->addItems(name_engine);
    ui->engineBox->setCurrentIndex(-1);
}
