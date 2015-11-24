#ifndef SEARCHRESULTLIST_H
#define SEARCHRESULTLIST_H

#include <QDialog>
#include <QString>
#include <QSqlTableModel>
#include <QSortFilterProxyModel>
#include <QDebug>
#include <QSqlError>
#include <QMenu>
#include <QKeyEvent>
#include "settings.h"
#include "firmdialog.h"

namespace Ui {
class SearchResultList;
}

class SearchResultList : public QDialog
{
    Q_OBJECT

public:
    explicit SearchResultList(QWidget *parent = 0);
    ~SearchResultList();
    void setSearch(QString text, QString append);

private slots:
    void on_tableView_customContextMenuRequested(const QPoint &pos);
    void openFirm();

private:
    Ui::SearchResultList    *ui;
    QSqlTableModel          *model;
    QSortFilterProxyModel   *proxy;
    Settings                *settings;
    QMenu                   *menu;

    // QWidget interface
protected:
    void keyPressEvent(QKeyEvent *event);
};

#endif // SEARCHRESULTLIST_H
