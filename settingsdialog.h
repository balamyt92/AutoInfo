#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QSettings>
#include <QString>
#include <QStringList>
#include <QMessageBox>
#include <QInputDialog>
#include "constants.h"

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();

private slots:
    void on_buttonBox_accepted();

    void on_comboBox_currentIndexChanged(int index);

private:
    Ui::SettingsDialog *ui;
    QSettings          *settings;
    QString             baseType;
};

#endif // SETTINGSDIALOG_H
