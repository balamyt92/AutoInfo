#ifndef BASEIMPORT_H
#define BASEIMPORT_H

#include <QObject>
#include <QWidget>
#include <QDebug>

#include <QString>
#include <QStringList>

#include <QFile>
#include <QTextStream>
#include <QFileDialog>

#include <QSqlQuery>
#include <QSqlError>

#include "constants.h"

class BaseImport : public QObject
{
    Q_OBJECT

private:
    QString          path;
    QString          baseName;
    QString          baseType;
    QList<QString>   files;
    QList<int>       columCount;

    bool insertData(QString fileName, QStringList data, QString filds, int count);
    bool clearTable(QString tableName);
    bool importFile(QString fileName, int coulumCount);

public:
    explicit BaseImport(QObject *parent = 0);
    ~BaseImport();

signals:
    void fail(QString msg_err);
    void messages(QString msg);
    void setMaxBar(int);
    void setValueToBar(int);

public slots:
    void setBaseType(QString baseType_);
    void setPathToFiles(QString path_);
    void setBaseName(QString base_);
    void startImport(QString baseName_);
};

#endif // BASEIMPORT_H
