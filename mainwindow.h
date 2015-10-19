#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QLabel>
#include <QPixmap>
#include <QTimer>

#include "database.h"
#include "settingsdialog.h"
#include "basewizarddialog.h"
#include "settings.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow  *ui;
    DataBase        database;
    Settings        *settings;
    QLabel          statusLine;
    QLabel          statusIcon;
    QTimer          *timer;

private slots:
    // Слот настройки формочки
    void configUi();
    // слот для таймера проверки статуса
    void statusCheck();

    // пункты меню
    void openSettings();
    void openBaseWizard();
};

#endif // MAINWINDOW_H
