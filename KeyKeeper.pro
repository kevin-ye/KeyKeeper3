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

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/cryptopp/release/ -lcryptopp
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/cryptopp/debug/ -lcryptopp
else:unix: LIBS += -L$$PWD/cryptopp/ -lcryptopp

INCLUDEPATH += $$PWD/cryptopp
DEPENDPATH += $$PWD/cryptopp

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/cryptopp/release/libcryptopp.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/cryptopp/debug/libcryptopp.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/cryptopp/release/cryptopp.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/cryptopp/debug/cryptopp.lib
else:unix: PRE_TARGETDEPS += $$PWD/cryptopp/libcryptopp.a
