#ifndef DETAILLIST_H
#define DETAILLIST_H

#include <QDialog>
#include <QString>
#include <QSqlTableModel>
#include <QSqlRelationalTableModel>
#include <QSortFilterProxyModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMenu>
#include <QList>
#include <QAction>
#include <QMessageBox>
#include <QInputDialog>
#include <QKeyEvent>

#include "settings.h"

namespace Ui {
class DetailList;
}

class DetailList : public QDialog
{
    Q_OBJECT

public:
    explicit DetailList(QWidget *parent = 0);
    ~DetailList();

private slots:
    void on_tableView_customContextMenuRequested(const QPoint &pos);
    void openDetailLink();
    void on_pushButton_clicked();
    void backToDetail();
    void addDetail();

private:
    Ui::DetailList           *ui;
    QSqlTableModel           *model;
    QSortFilterProxyModel    *proxy;
    Settings                 *settings;
    QMenu                    *menu;
    QSqlRelationalTableModel *rmodel;
    bool                     inLinked;
    int                      selectRow;

protected:
    void keyPressEvent(QKeyEvent *event);
};

#endif // DETAILLIST_H
