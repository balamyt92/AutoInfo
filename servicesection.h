#ifndef SERVICESECTION_H
#define SERVICESECTION_H

#include <QWidget>
#include <QString>
#include <QSqlTableModel>
#include <QSortFilterProxyModel>
#include <QSqlError>
#include <QDebug>
#include <QMenu>
#include <QList>
#include <QAction>
#include <QMessageBox>
#include <QInputDialog>
#include <QKeyEvent>

#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QGridLayout>
#include <QDialogButtonBox>
#include <QSqlQuery>

#include "settings.h"
#include "servicesearchresult.h"

namespace Ui {
class ServiceSection;
}

class ServiceSection : public QWidget
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
    void editSection();

public slots:
    void selectData();

protected:
    void keyPressEvent(QKeyEvent *event);
};

#endif // SERVICESECTION_H
