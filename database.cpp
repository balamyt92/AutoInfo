#include "database.h"

DataBase::DataBase(QObject *parent) : QObject(parent)
{
    settings = Settings::getInstance();
}

DataBase::~DataBase()
{

}

void DataBase::connectToDataBase()
{
    // определям к какой базе будем подключаться
    // на выбор у нас база sqlite (локальная) или mysql (сервер)
    // по умолчанию подключаемся к серверу, иначе пробуем подключиться к локальной базе
    baseType = settings->value("base/type", SERVER).toString();
    if(!baseType.isEmpty())
    {
        if(baseType == SERVER)
        {
            if(!this->openServerBase())
            {
                qDebug() << tr("Соединение с сервером не установленно! Перехожу в локальную работу.");
                QMessageBox msgBox;
                msgBox.setText("Не удалось соедениться с базой данных!");
                msgBox.setInformativeText(this->getErr() + " Пытаюсь перейти в локальную работу");
                msgBox.setIcon(QMessageBox::Warning);
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.setDefaultButton(QMessageBox::Ok);
                msgBox.exec();
                settings->setValue("base/type", LOCAL);
                this->connectToDataBase();
            }
            else
            {
                QString baseName = settings->value("server/basename", "").toString();
                if(!baseName.isEmpty())
                {
                    db.close();
                    db.setDatabaseName(baseName);
                    db.open();
                }
            }
        }
        else
        {
            if(baseType == LOCAL)
            {
                if(!this->openLocalBase())
                {
                    qDebug() << tr("Соединение с локальной базой не установленно!");
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
                qDebug() << tr("Критичесская ошиибка соедниения! Проверте настройки! Не верен base/type");
            }
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
        name = "<b>NO BASE</b>";
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

bool DataBase::openServerBase()
{
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(settings->value("server/hostname","localhost").toString());
    db.setUserName(settings->value("server/username","root").toString());
    db.setPassword(settings->value("server/password","data").toString());
    db.setPort(settings->value("server/port","3306").toInt());
    if(db.open())
    {
        qDebug() << tr("Соедниение с сервером установлено!");
        return true;
    }
    else
    {
        qDebug() << tr("Ошибка соеднеиния!");
        return false;
    }
}

bool DataBase::openLocalBase()
{
    if(!QFile(settings->value("localbase/basePath", "base.sqlite").toString()).exists())
    {
        qDebug() << tr("Ошибка соеднеиния! Нет файла базы!");
        return false;
    }
    else
    {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setHostName(settings->value("localbase/hostname", "localhost").toString());
        db.setDatabaseName(settings->value("localbase/basePath", "base.sqlite").toString());
        if(db.open())
        {
            qDebug() << tr("Соедниение с локальной базой установлено!");
            return true;
        }
        else
        {
            qDebug() << tr("Ошибка соеднеиния!");
            return false;
        }
    }
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

