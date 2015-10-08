#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QMessageBox>
#include <QLabel>
#include <QPixmap>
#include <QTimer>
#include "database.h"
#include "settingsdialog.h"

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
    QSettings       *settings;
    QLabel          statusLine;
    QLabel          statusIcon;

private slots:
    // Слот настройки формочки
    void configUi();
    void statusCheck();
    void openSettings();
};

#endif // MAINWINDOW_H
