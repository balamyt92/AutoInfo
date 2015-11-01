#ifndef FILTERDIALOG_H
#define FILTERDIALOG_H

#include <QDialog>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QStringList>
#include <QCompleter>

namespace Ui {
class FilterDialog;
}

class FilterDialog : public QDialog
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

private:
    Ui::FilterDialog    *ui;
    QStringList         id_mark;
    QStringList         name_mark;
    QCompleter          *mark;

    QStringList         id_model;
    QStringList         name_model;
    QCompleter          *model;

    QStringList         id_body;
    QStringList         name_body;
    QCompleter          *body;

    QStringList         id_engine;
    QStringList         name_engine;
    QCompleter          *engine;

    QStringList         id_detail;
    QStringList         name_detail;
    QCompleter          *detail;



};

#endif // FILTERDIALOG_H
