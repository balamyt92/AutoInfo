#ifndef FILTERRESULT_H
#define FILTERRESULT_H

#include <QDialog>
#include <QMenu>
#include <QSqlTableModel>
#include <QSqlError>
#include <QDebug>
#include "settings.h"
#include "firmdialog.h"
#include "filterresulpproxymodel.h"

namespace Ui {
class FilterResult;
}

class FilterResult : public QDialog
{
    Q_OBJECT

public:
    explicit FilterResult(QWidget *parent = 0);
    ~FilterResult();
    void setModel(QAbstractItemModel *model);

private slots:
    void on_tableView_customContextMenuRequested(const QPoint &pos);
    void openFirm();
    void changeColor();

private:
    Ui::FilterResult    *ui;
    QMenu               *menu;
    Settings            *settings;
    FilterResulpProxyModel *proxy;

protected:
    void keyPressEvent(QKeyEvent *event);

};

#endif // FILTERRESULT_H
