#include "database.h"

DataBase::DataBase(QObject *parent) : QObject(parent)
{

}

DataBase::~DataBase()
{

}

void DataBase::connectToDataBase(QSettings *settings_)
{
    // определям к какой базе будем подключаться
    // на выбор у нас база sqlite (локальная) или mysql (сервер)
    this->settings = settings_;
    baseType = settings->value("base/type", SERVER).toString();
    if(!baseType.isEmpty())
    {
        if(!this->openDataBase())
        {
            qDebug() << tr("Соединение с базой не установленно!");
            qDebug() << tr("Проверьте настройки");
            QMessageBox msgBox;
            msgBox.setText("Не удалось соедениться с базой данных!");
            msgBox.setInformativeText(this->getErr());
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.exec();
        }
    }
    else
    {
        qDebug() << tr("ОШИБКА!: не указан тип базы, проверте настройки.");
    }

}

QString DataBase::getBaseName()
{
    QString name;
    if(db.databaseName().isEmpty() || db.databaseName().isNull() || db.databaseName() == " ")
        name = "NO BASE";
    else
        name = db.databaseName();
    return name;
}

QString DataBase::getBaseType()
{
    QString type;
    if(db.driverName() == "QMYSQL")
        type = "SERVER";
    if(db.driverName() == "QSQLITE")
        type = "LOCAL";
    return type;
}

bool DataBase::getConnectStatus()
{
    return db.isOpen();
}

QString DataBase::getErr()
{
    return db.lastError().text();
}

bool DataBase::openDataBase()
{
    if(baseType == LOCAL)
    {
        if(!QFile(LOCAL_BASE_NAME).exists())
        {
            return false;
        }
        else
        {
            db = QSqlDatabase::addDatabase("QSQLITE");
            db.setHostName(settings->value("localbase/hostname", "localhost").toString());
            db.setDatabaseName(settings->value("localbase/basename", LOCAL_BASE_NAME).toString());
            if(db.open())
                return true;
            else
                return false;
        }
    }
    if(baseType == SERVER)
    {
        db = QSqlDatabase::addDatabase("QMYSQL");
        db.setHostName(settings->value("server/hostname","localhost").toString());
        db.setUserName(settings->value("server/username","root").toString());
        db.setPassword(settings->value("server/password","data").toString());
        db.setPort(settings->value("server/port","3306").toInt());
        if(db.open())
        {
            if(!settings->value("server/basename").isNull())
            {
                db.setDatabaseName(settings->value("server/basename").toString());
            }
            else
            {
                this->selectBase();
            }
            return true;
        }
        else
            return false;
    }
    return false;
}

void DataBase::closeDataBase()
{
    db.close();
}

void DataBase::selectBase()
{
    QStringList baselist;
    baselist.clear();
    QSqlQuery sql("show databases");
    while(sql.next())
    {
        baselist << sql.value(0).toString();
    }
    bool ok;
    QString base = QInputDialog::getItem(qobject_cast<QWidget *>(this), tr("Выберите базу данных"), tr("Выберите базу"), baselist, 0, true, &ok);
    if(ok && !base.isEmpty())
    {
        db.setDatabaseName(base);
        settings->setValue("server/basename", base);
        settings->sync();
    }
    else
    {
        QMessageBox::warning(qobject_cast<QWidget *>(this),tr("Предупреждение"), tr("Вы не выбрали базу данных! Соединение будет разорванно!"), QMessageBox::Ok, QMessageBox::Ok);
    }
}

