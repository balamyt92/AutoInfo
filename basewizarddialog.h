#ifndef BASEWIZARDDIALOG_H
#define BASEWIZARDDIALOG_H

#include <QDialog>
#include <QDebug>

#include <QSqlQueryModel>
#include <QSortFilterProxyModel>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlError>

#include <QString>
#include <QStringList>

#include <QMessageBox>
#include <QInputDialog>

#include <QThread>
#include <QtConcurrent/QtConcurrent>
#include <QFuture>
#include <QFutureWatcher>

#include "constants.h"
#include "baseimport.h"
#include "settings.h"

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
    Settings              *settings;
    QString                baseType;
    QSqlQueryModel        *model;
    QSortFilterProxyModel *proxyModel;
    QThread                thread;

private slots:
    QString getSelectBase();
    void on_creatButton_clicked();
    void on_deleteButton_clicked();
    void on_importButton_clicked();
    void updateBaseList();
    void disabledUi();
    void enabledUi();

public slots:
    void setMsgToLog(QString msg);

signals:
    void startImport(QString basename);
};

#endif // BASEWIZARDDIALOG_H
