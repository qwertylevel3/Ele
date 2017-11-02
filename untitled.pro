#-------------------------------------------------
#
# Project created by QtCreator 2017-10-08T16:19:32
#
#-------------------------------------------------

QT       += core gui

QT		 += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = untitled
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ele.cpp \
    eleitem.cpp \
    importdialog.cpp \
    evadialog.cpp \
    atrdialog.cpp \
    simpledate.cpp \
    util.cpp \
    evarepdialog.cpp \
    repdialog.cpp

HEADERS  += mainwindow.h \
    csv.h \
    ele.h \
    eleitem.h \
    importdialog.h \
    evadialog.h \
    atrdialog.h \
    simpledate.h \
    util.h \
    evarepdialog.h \
    repdialog.h

FORMS    += mainwindow.ui \
    importdialog.ui \
    evadialog.ui \
    atrdialog.ui \
    evarepdialog.ui \
    repdialog.ui
