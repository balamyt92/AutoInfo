#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QLabel>
#include <QPixmap>
#include <QTimer>
#include <QKeyEvent>

#include "database.h"
#include "settingsdialog.h"
#include "basewizarddialog.h"
#include "settings.h"
#include "searchresultlist.h"
#include "servicesection.h"
#include "filterdialog.h"
#include "firmslist.h"
#include "detaillist.h"
#include "searchform.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void activateServiceWindows();
    void activateSearchWindows();

private:
    Ui::MainWindow  *ui;
    DataBase        database;
    Settings        *settings;
    QLabel          statusLine;
    QLabel          statusIcon;
    QTimer          *timer;
    FilterDialog    *filterDialog;
    ServiceSection  *serviceDialog;
    SearchForm      *searchDialog;

private slots:
    // слот для таймера проверки статуса
    void statusCheck();

    // пункты меню
    void openSettings();
    void openBaseWizard();
    void openDetail();
    void openFirms();

protected:
    void keyPressEvent(QKeyEvent *e);
};

#endif // MAINWINDOW_H
