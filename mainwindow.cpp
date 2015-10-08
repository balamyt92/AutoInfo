#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    settings = new QSettings("settings.ini", QSettings::IniFormat);
    this->configUi();
}

MainWindow::~MainWindow()
{
    delete ui;
    settings->setValue("MainWindows/geometry", this->saveGeometry());

    settings->sync();
}

void MainWindow::configUi()
{
    this->setWindowTitle("Автомобильная справочная служба");
    this->restoreGeometry(settings->value("MainWindows/geometry").toByteArray());
}
