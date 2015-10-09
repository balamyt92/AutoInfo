#ifndef BASEWIZARDDIALOG_H
#define BASEWIZARDDIALOG_H

#include <QDialog>
#include <QString>
#include <QSqlQueryModel>
#include <QSettings>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
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
    Ui::BaseWizardDialog *ui;
    QSettings            *settings;
    QString               baseType;
    QSqlQueryModel       *model;
};

#endif // BASEWIZARDDIALOG_H
