#ifndef PRICELIST_H
#define PRICELIST_H

#include <QDialog>
#include <QString>
#include <QSqlRelationalTableModel>
#include <QSortFilterProxyModel>
#include <QSqlError>
#include <QDebug>
#include <QMenu>
#include <QList>
#include <QAction>
#include <QMessageBox>
#include <QKeyEvent>
#include "settings.h"

namespace Ui {
class PriceList;
}

class PriceList : public QDialog
{
    Q_OBJECT

public:
    explicit PriceList(QWidget *parent = 0);
    ~PriceList();
    bool setFirmID(QString id);

private:
    Ui::PriceList               *ui;
    QSqlRelationalTableModel    *model;
    //QSortFilterProxyModel       *proxy;
    QMenu                       *menu;
    Settings                    *settings;
    QString                     ID;

public slots:
    void enableSort(int colum);
};

#endif // PRICELIST_H
