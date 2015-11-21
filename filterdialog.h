#ifndef FILTERDIALOG_H
#define FILTERDIALOG_H

#include <QWidget>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QStringList>
#include <QSqlQueryModel>

#include <QSortFilterProxyModel>
#include "filterresult.h"

namespace Ui {
class FilterDialog;
}

class FilterDialog : public QWidget
{
    Q_OBJECT

public:
    explicit FilterDialog(QWidget *parent = 0);
    ~FilterDialog();

private slots:
    void on_searchButton_clicked();
    void setModeles();
    void setBodys();
    void setEngines();

public slots:
    void selectBeginData();

private:
    Ui::FilterDialog    *ui;

    QStringList         id_mark;
    QStringList         name_mark;

    QStringList         id_model;
    QStringList         name_model;

    QStringList         id_body;
    QStringList         name_body;

    QStringList         id_engine;
    QStringList         name_engine;

    QStringList         id_detail;
    QStringList         name_detail;
};

#endif // FILTERDIALOG_H
