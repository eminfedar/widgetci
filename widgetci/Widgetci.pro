#-------------------------------------------------
#
# Project created by QtCreator 2017-06-21T19:13:09
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets script

TARGET = Widgetci
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    w_widget.cpp

HEADERS  += mainwindow.h \
    w_widget.h

FORMS    += mainwindow.ui \
    w_widget.ui

QMAKE_CXXFLAGS += -std=c++11

RESOURCES += \
    qtresource.qrc
