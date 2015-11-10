#ifndef FIRMSLIST_H
#define FIRMSLIST_H

#include <QDialog>
#include <QString>
#include <QSqlTableModel>
#include <QSortFilterProxyModel>
#include <QSqlError>
#include <QDebug>
#include <QMenu>
#include <QList>
#include <QAction>
#include <QMessageBox>
#include <QKeyEvent>

#include "settings.h"
#include "firmproxymodel.h"

namespace Ui {
class FirmsList;
}

class FirmsList : public QDialog
{
    Q_OBJECT

public:
    explicit FirmsList(QWidget *parent = 0);
    ~FirmsList();

private slots:
    void on_tableView_customContextMenuRequested(const QPoint &pos);

    void on_filtrButton_clicked();

    void openPrice();
    void openServices();
    void addFirm();
    void deleteFirm();
    void editFirm();

private:
    Ui::FirmsList           *ui;
    QSqlTableModel          *model;
    FirmProxyModel          *proxy;
    QMenu                   *menu;
    Settings                *settings;

};

#endif // FIRMSLIST_H
