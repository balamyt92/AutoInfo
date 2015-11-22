#-------------------------------------------------
#
# Project created by QtCreator 2015-10-08T19:10:50
#
#-------------------------------------------------

QT       += core gui sql concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AutoInfo
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    database.cpp \
    settingsdialog.cpp \
    basewizarddialog.cpp \
    baseimport.cpp \
    settings.cpp \
    searchresultlist.cpp \
    firmdialog.cpp \
    servicesection.cpp \
    servicesearchresult.cpp \
    filterdialog.cpp \
    detaillist.cpp \
    firmslist.cpp \
    firmproxymodel.cpp \
    searchform.cpp \
    filterresult.cpp \
    filterresulpproxymodel.cpp \
    colordelegate.cpp

HEADERS  += mainwindow.h \
    database.h \
    settingsdialog.h \
    constants.h \
    basewizarddialog.h \
    baseimport.h \
    settings.h \
    searchresultlist.h \
    firmdialog.h \
    servicesection.h \
    servicesearchresult.h \
    filterdialog.h \
    detaillist.h \
    firmslist.h \
    firmproxymodel.h \
    searchform.h \
    filterresult.h \
    filterresulpproxymodel.h \
    colordelegate.h

FORMS    += mainwindow.ui \
    settingsdialog.ui \
    basewizarddialog.ui \
    searchresultlist.ui \
    firmdialog.ui \
    servicesection.ui \
    servicesearchresult.ui \
    filterdialog.ui \
    detaillist.ui \
    firmslist.ui \
    searchform.ui \
    filterresult.ui

RESOURCES += \
    res.qrc
