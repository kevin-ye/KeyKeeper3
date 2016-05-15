#-------------------------------------------------
#
# Project created by QtCreator 2016-05-15T16:06:55
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = KeyKeeper
TEMPLATE = app


SOURCES += main.cpp\
        logindialog.cpp \
    databasehandler.cpp

HEADERS  += logindialog.h \
    databasehandler.h

FORMS    += logindialog.ui
