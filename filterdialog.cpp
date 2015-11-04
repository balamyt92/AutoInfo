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

    connect(ui->markBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setModeles()));
    connect(ui->markBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setEngines()));
}

FilterDialog::~FilterDialog()
{
    delete ui;
}

void FilterDialog::on_searchButton_clicked()
{
    // уходим если не соблюден минимальный набор данных
    if(ui->markBox->currentIndex() == -1 || ui->detailBox->currentIndex() == -1)
        return;

    QString where;
    QSqlQuery query("SELECT ID FROM carmarksen WHERE Name LIKE \'***\'");
    query.next();
    // нам нужны помимо выбранной марки еще и три звезды которые обозначают любую марку
    where = "(carpresenceen.ID_Mark=" + id_mark.at(ui->markBox->currentIndex()) +
            " OR carpresenceen.ID_Mark="+ query.value(0).toString() +") AND (";


    // смотрим с какими деталями связана выбранная, они нам так же нужны в запросе
    query.prepare("SELECT ID_LinkedDetail FROM carenlinkeddetailnames WHERE ID_GroupDetail="
                  + id_detail.at(ui->detailBox->currentIndex()));
    query.exec();

    where += "carpresenceen.ID_Name=" + id_detail.at(ui->detailBox->currentIndex());
    if(query.isValid())
    {
        while (query.next())
        {
            where += " OR carpresenceen.ID_Name=" + query.value(0).toString();
        }
    }

    // указана ли модель в фильтре
    if(ui->modelBox->currentIndex() != -1)
    {
        where += ") AND (";

        // проверяем являеться ли модель группой моделей
        QString model_id = id_model.at(ui->modelBox->currentIndex());
        query.prepare("SELECT ID_Model FROM carmodelgroupsen"
                      "WHERE ID_Group=" + model_id);
        query.exec();

        where += "carpresenceen.ID_Model=" + model_id;
        if(query.isValid())
        {
            while (query.next())
            {
                where += " OR carpresenceen.ID_Model=" + query.value(0).toString();
            }
        }

        // нам нужна модель три звездочки обозначающая все модели марки
        query.prepare("SELECT ID FROM carmarksen WHERE Name=\'***\' AND ID_Mark="
                      + id_mark.at(ui->markBox->currentIndex()));
        query.exec();
        if(query.isValid())
        {
            where += " OR carpresenceen.ID_Model=" + query.value(0).toString();
        }

    }

    // указан ли кузов в фильтре
    if(ui->bodyBox->currentIndex() != -1)
    {
        where += ") AND (";

        // проверяем являеться ли кузов группой кузовов
        QString body_id = id_body.at(ui->bodyBox->currentIndex());
        query.prepare("SELECT ID_BodyModel FROM carbodymodelgroupsen"
                      "WHERE ID_BodyGroup=" + body_id);
        query.exec();

        where += "carpresenceen.ID_Body=" + body_id;
        if(query.isValid())
        {
            while (query.next())
            {
                where += " OR carpresenceen.ID_Body=" + query.value(0).toString();
            }
        }

        // нам нужен кузов три звездочки обозначающий все кузова модели
        query.prepare("SELECT ID FROM carbodymodelsen WHERE Name=\'***\' AND ID_Mark="
                      + id_mark.at(ui->markBox->currentIndex()) +
                      " AND ID_Model=" + id_model.at(ui->modelBox->currentIndex()));
        query.exec();
        if(query.isValid())
        {
            where += " OR carpresenceen.ID_Body=" + query.value(0).toString();
        }
    }

    // указан ли двигатель в фильтре
    if(ui->engineBox->currentIndex() != -1)
    {
        where += ") AND (";

        // проверяем являеться ли двигатель группой кузовов
        QString engine_id = id_engine.at(ui->engineBox->currentIndex());
        query.prepare("SELECT ID_EngineModel FROM carenginemodelgroupsen"
                      "WHERE ID_EngineGroup=" + engine_id);
        query.exec();

        where += "carpresenceen.ID_Engine=" + engine_id;
        if(query.isValid())
        {
            while (query.next())
            {
                where += " OR carpresenceen.ID_Engine=" + query.value(0).toString();
            }
        }

        // нам нужен двигатель три звездочки обозначающий все двигателя модели
        query.prepare("SELECT ID FROM carenginemodelsen WHERE Name=\'***\' AND ID_Mark="
                      + id_mark.at(ui->markBox->currentIndex()));
        query.exec();
        if(query.isValid())
        {
            where += " OR carpresenceen.ID_Engine=" + query.value(0).toString();
        }
    }

    where += ")";


    QSqlQueryModel *result = new QSqlQueryModel(this);
    result->setQuery("SELECT carpresenceen.ID_Firm, carmarksen.Name,"
                     "carmodelsen.Name, carbodymodelsen.Name, carenginemodelsen.Name,"
                     "carpresenceen.Comment, carpresenceen.TechNumber,"
                     "carpresenceen.Catalog_Number, carpresenceen.Cost "
                     "FROM carpresenceen "
                     "LEFT JOIN carmarksen "
                     "ON carpresenceen.ID_Mark=carmarksen.ID "
                     "LEFT JOIN carmodelsen "
                     "ON carpresenceen.ID_Model=carmodelsen.ID "
                     "LEFT JOIN carbodymodelsen "
                     "ON carpresenceen.ID_Body=carbodymodelsen.ID "
                     "LEFT JOIN carenginemodelsen "
                     "ON carpresenceen.ID_Engine=carenginemodelsen.ID "
                     "WHERE "+ where);

    QDialog *d = new QDialog(this);
    QTableView *v = new QTableView(d);
    v->setModel(result);
    QGridLayout *grid = new QGridLayout(d);
    grid->addWidget(v,0,0,0,0);
    d->setLayout(grid);
    d->setWindowFlags(Qt::Window);
    d->exec();
    delete d;

}

void FilterDialog::setModeles()
{
    if(id_model.isEmpty())
    {
        ui->modelBox->setEnabled(true);
        ui->engineBox->setEnabled(true);
    }
    else
    {
        id_model.clear();
        name_model.clear();
        if(ui->bodyBox->isEnabled())
            ui->bodyBox->setDisabled(true);
    }

    QSqlQuery query("SELECT ID, Name FROM carmodelsen WHERE ID_Mark="
                    + id_mark.at(ui->markBox->currentIndex()) +
                    " ORDER BY Name ASC");
    while(query.next())
    {
        id_model << query.value(0).toString();
        name_model << query.value(1).toString();
    }

    this->disconnect(ui->modelBox, SIGNAL(currentIndexChanged(int)), this, 0);
    this->disconnect(ui->bodyBox, SIGNAL(currentIndexChanged(int)), this, 0);

    ui->modelBox->clear();
    ui->modelBox->addItems(name_model);
    ui->modelBox->setCurrentIndex(-1);

    connect(ui->modelBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setBodys()));
    connect(ui->bodyBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setEngines()));
}

void FilterDialog::setBodys()
{
    if(ui->modelBox->currentIndex() == -1)
        return;

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
    ui->bodyBox->setEnabled(true);
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
        if(ui->modelBox->currentIndex() >= 0)
        {
            query.prepare("SELECT carenginemodelsen.ID, carenginemodelsen.Name FROM carengineandmodelcorrespondencesen"
                          " LEFT JOIN carenginemodelsen"
                          " ON carengineandmodelcorrespondencesen.ID_Engine=carenginemodelsen.ID"
                          " WHERE carengineandmodelcorrespondencesen.ID_Mark=" + id_mark.at(ui->markBox->currentIndex()) +
                          " AND carengineandmodelcorrespondencesen.ID_Model=" + id_model.at(ui->modelBox->currentIndex()) +
                          " ORDER BY Name ASC");
        }
        else
        {
            query.prepare("SELECT ID, Name FROM carenginemodelsen"
                          " WHERE ID_Mark=" + id_mark.at(ui->markBox->currentIndex()) +
                          " ORDER BY Name ASC");
        }
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
    ui->engineBox->setEnabled(true);
}

