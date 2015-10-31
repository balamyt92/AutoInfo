#ifndef SERVICESECTION_H
#define SERVICESECTION_H

#include <QDialog>
#include <QString>
#include <QSqlTableModel>
#include <QSortFilterProxyModel>
#include <QSqlError>
#include <QDebug>
#include <QMenu>
#include <QList>
#include <QAction>
#include "settings.h"

namespace Ui {
class ServiceSection;
}

class ServiceSection : public QDialog
{
    Q_OBJECT

public:
    explicit ServiceSection(QWidget *parent = 0);
    ~ServiceSection();

private:
    Ui::ServiceSection      *ui;
    Settings                *settings;
    QSqlTableModel          *model;
    QSortFilterProxyModel   *proxy;
    QMenu                   *menu;
    bool                    sectionIsOpen;
    int                     selectedSection;
    QString                 id;

private slots:
    void addSection();
    void deleteSection();
    void openSection();
    void on_tableView_customContextMenuRequested(const QPoint &pos);
    void backToSections();

protected:
    void keyPressEvent(QKeyEvent *event);
};

#endif // SERVICESECTION_H
