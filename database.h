#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSql>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QFile>
#include <QDebug>
#include <QInputDialog>
#include <QMessageBox>
#include "settings.h"

#include "constants.h"

class DataBase : public QObject
{
    Q_OBJECT
public:
    explicit DataBase(QObject *parent = 0);
    ~DataBase();
    void connectToDataBase();
    QString getBaseName();
    QString getBaseType();
    bool getConnectStatus();
    QString getErr();
    void closeDataBase();
    void selectBase();

private:
    QString         baseType;
    Settings        *settings;
    QSqlDatabase    db;

    bool openServerBase();
    bool openLocalBase();

signals:

public slots:
};

#endif // DATABASE_H
