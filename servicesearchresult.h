#ifndef SERVICESEARCHRESULT_H
#define SERVICESEARCHRESULT_H

#include <QDialog>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QString>
#include <QMenu>
#include <QEvent>
#include <QDebug>
#include "settings.h"
#include "firmdialog.h"

namespace Ui {
class ServiceSearchResult;
}

class ServiceSearchResult : public QDialog
{
    Q_OBJECT

public:
    explicit ServiceSearchResult(QWidget *parent = 0);
    ~ServiceSearchResult();
    void setServiceId(int id_service);

private slots:
    void on_tableView_customContextMenuRequested(const QPoint &pos);
    void openFirm();

private:
    Ui::ServiceSearchResult     *ui;
    QSqlQueryModel              *model;
    Settings                    *settings;
    QMenu                       *menu;

protected:
    void keyPressEvent(QKeyEvent *event);

};

#endif // SERVICESEARCHRESULT_H
