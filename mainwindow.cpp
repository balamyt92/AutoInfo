#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    settings = Settings::getInstance();
    this->setWindowTitle("Автомобильная справочная служба");
    this->restoreGeometry(settings->value("MainWindows/geometry").toByteArray());
    database.setParent(this);
    database.connectToDataBase();
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(statusCheck()));
    timer->start(1000);
    this->statusCheck();
    ui->statusBar->addWidget(&statusIcon);
    ui->statusBar->addWidget(&statusLine);


    connect(ui->actionSettings,SIGNAL(triggered(bool)),this, SLOT(openSettings()));
    connect(ui->actionBase,SIGNAL(triggered(bool)),this, SLOT(openBaseWizard()));
    connect(ui->actionServices, SIGNAL(triggered(bool)), this, SLOT(on_serviceButton_clicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
    settings->setValue("MainWindows/geometry", this->saveGeometry());
    database.closeDataBase();
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
}

void MainWindow::openBaseWizard()
{
    BaseWizardDialog *bwd = new BaseWizardDialog(this);
    bwd->restoreGeometry(settings->value("BaseWizardDialog/geometry").toByteArray());
    bwd->exec();
    settings->setValue("BaseWizardDialog/geometry", bwd->saveGeometry());
    delete bwd;
}

void MainWindow::on_searchButton_clicked()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Поиск по фирмам"), tr("Что ищем:"),
                                         QLineEdit::Normal, "", &ok);
    if(ok && !text.isEmpty())
    {
        SearchResultList * sr = new SearchResultList(this);
        sr->setSearch(text);
        sr->exec();
        delete sr;
    }
}

void MainWindow::on_serviceButton_clicked()
{
    ServiceSection *ss = new ServiceSection(this);
    ss->exec();
    delete ss;
}
