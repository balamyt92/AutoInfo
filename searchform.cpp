#include "searchform.h"
#include "ui_searchform.h"
#include "searchresultlist.h"

SearchForm::SearchForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::searchForm)
{
    ui->setupUi(this);
    settings = Settings::getInstance();
    this->setWindowTitle("Поиск");
    this->restoreGeometry(settings->value("searchForm/geometry").toByteArray());
    this->setWindowFlags(Qt::Window | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint | Qt::CustomizeWindowHint);
    ui->lineEdit->setFocus();
}

SearchForm::~SearchForm()
{
    settings->setValue("searchForm/geometry", this->saveGeometry());
    delete ui;
}

void SearchForm::on_pushButton_clicked()
{
    if(!ui->lineEdit->text().isEmpty())
    {
        SearchResultList * sr = new SearchResultList(this);
        sr->setSearch(ui->lineEdit->text().replace("  ", " "));
        sr->exec();
        delete sr;
        ui->lineEdit->selectAll();
    }
}

#include "mainwindow.h"
void SearchForm::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return)
    {
        on_pushButton_clicked();
    }

    if(e->key() == Qt::Key_F1 || e->key() == Qt::Key_F2)
    {
        this->parentWidget()->activateWindow();
    }
    if(e->key() == Qt::Key_F3)
    {
        MainWindow *main = qobject_cast<MainWindow*>(this->parent());
        main->activateServiceWindows();
    }
    QWidget::keyPressEvent(e);
}
