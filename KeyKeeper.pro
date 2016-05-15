#-------------------------------------------------
#
# Project created by QtCreator 2016-05-15T16:06:55
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = KeyKeeper
TEMPLATE = app


SOURCES += main.cpp\
        logindialog.cpp \
    databasehandler.cpp \
    mainwindow.cpp

HEADERS  += logindialog.h \
    databasehandler.h \
    mainwindow.h \
    common.h

FORMS    += logindialog.ui \
    mainwindow.ui
