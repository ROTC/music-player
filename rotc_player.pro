#-------------------------------------------------
#
# Project created by QtCreator 2014-02-19T21:23:56
#
#-------------------------------------------------

QT       += core gui \
            multimedia
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = rotc_player
TEMPLATE = app


SOURCES += main.cpp\
        mywidget.cpp \
    myplaylist.cpp

HEADERS  += mywidget.h \
    myplaylist.h

FORMS    += mywidget.ui

RESOURCES += \
    images.qrc
