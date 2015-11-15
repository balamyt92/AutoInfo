#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QString>
#include <QStringList>
#include <QMessageBox>
#include <QInputDialog>
#include "constants.h"
#include "settings.h"

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

    void on_pathButton_clicked();

private:
    Ui::SettingsDialog *ui;
    Settings           *settings;
    QString             baseType;
};

#endif // SETTINGSDIALOG_H
