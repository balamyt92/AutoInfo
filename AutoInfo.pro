#-------------------------------------------------
#
# Project created by QtCreator 2015-10-08T19:10:50
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AutoInfo
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    database.cpp \
    settingsdialog.cpp

HEADERS  += mainwindow.h \
    database.h \
    settingsdialog.h \
    constants.h

FORMS    += mainwindow.ui \
    settingsdialog.ui

RESOURCES += \
    res.qrc
