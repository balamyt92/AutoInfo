#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("Автомобильная справочная служба");
    QCoreApplication::setOrganizationDomain("www.autoinfo24.ru");
    QCoreApplication::setApplicationName("AutoInfo");
    QCoreApplication::setApplicationVersion("0.1 pre alfa");

    MainWindow w;
    w.show();

    return a.exec();
}
