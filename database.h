#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSql>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QFile>
#include <QSettings>
#include <QDebug>
#include <QInputDialog>
#include <QMessageBox>

#include "constants.h"

class DataBase : public QObject
{
    Q_OBJECT
public:
    explicit DataBase(QObject *parent = 0);
    ~DataBase();
    void connectToDataBase(QSettings *settings_);
    QString getBaseName();
    QString getBaseType();
    bool getConnectStatus();
    QString getErr();
    void closeDataBase();
    void selectBase();

private:
    QString         baseType;
    QSettings       *settings;
    QSqlDatabase    db;

    bool openDataBase();

signals:

public slots:
};

#endif // DATABASE_H
