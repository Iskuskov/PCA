#-------------------------------------------------
#
# Project created by QtCreator 2014-10-17T10:04:13
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PCA
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
        paintarea.cpp \
    princomp.cpp

HEADERS  += mainwindow.h \
            paintarea.h \
    princomp.h

FORMS    += mainwindow.ui
