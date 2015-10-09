#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    settings = new QSettings(CONFIG_FILE, QSettings::IniFormat);
    this->configUi();
    database.setParent(this);
    database.connectToDataBase();
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(statusCheck()));
    timer->start();
    this->statusCheck();
    ui->statusBar->addWidget(&statusIcon);
    ui->statusBar->addWidget(&statusLine);


    connect(ui->actionSettings,SIGNAL(triggered(bool)),this, SLOT(openSettings()));
}

MainWindow::~MainWindow()
{
    delete ui;
    settings->setValue("MainWindows/geometry", this->saveGeometry());
    settings->sync();
    database.closeDataBase();
}

void MainWindow::configUi()
{
    this->setWindowTitle("Автомобильная справочная служба");
    this->restoreGeometry(settings->value("MainWindows/geometry").toByteArray());
}

void MainWindow::statusCheck()
{
    if(database.getConnectStatus())
        statusIcon.setPixmap(QPixmap(":/icons/statusTrue.png"));
    else
        statusIcon.setPixmap(QPixmap(":/icons/statusFalse.png"));

    statusLine.setText(database.getBaseName() + " : " + database.getBaseType());
}

void MainWindow::openSettings()
{
    SettingsDialog *sd = new SettingsDialog();
    sd->restoreGeometry(settings->value("SettingsDialog/geometry").toByteArray());
    if(sd->exec() == QDialog::Accepted)
    {
        database.closeDataBase();
        database.connectToDataBase();
        if(settings->value("base/type") == SERVER && database.getConnectStatus())
        {
            database.selectBase();
        }
    }
    settings->setValue("SettingsDialog/geometry", sd->saveGeometry());
    delete sd;
    settings->sync();
}
