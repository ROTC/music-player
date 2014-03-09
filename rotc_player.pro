#-------------------------------------------------
#
# Project created by QtCreator 2014-02-19T21:23:56
#
#-------------------------------------------------

QT       += core gui \
            multimedia\
            widgets\
            sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = rotc_player
TEMPLATE = app


SOURCES += main.cpp\
        mywidget.cpp \
    myplaylist.cpp \
    mtag.cpp \
    mylrc.cpp \
    DBModule.cpp

HEADERS  += mywidget.h \
    myplaylist.h \
    mtag.h \
    mylrc.h \
    DBModule.h \
    head.h

FORMS    += mywidget.ui

RESOURCES += \
    images.qrc
INCLUDEPATH += /usr/local/include/taglib

LIBS += -L/usr/local/lib -ltag
