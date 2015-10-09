#ifndef BASEWIZARDDIALOG_H
#define BASEWIZARDDIALOG_H

#include <QDialog>
#include <QString>
#include <QSqlQueryModel>
#include <QSortFilterProxyModel>
#include <QSettings>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QInputDialog>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include "constants.h"

namespace Ui {
class BaseWizardDialog;
}

class BaseWizardDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BaseWizardDialog(QWidget *parent = 0);
    ~BaseWizardDialog();

private:
    Ui::BaseWizardDialog  *ui;
    QSettings             *settings;
    QString               baseType;
    QSqlQueryModel        *model;
    QSortFilterProxyModel *proxyModel;

private slots:
    QString getSelectBase();
    void on_creatButton_clicked();
    void on_deleteButton_clicked();
};

#endif // BASEWIZARDDIALOG_H
