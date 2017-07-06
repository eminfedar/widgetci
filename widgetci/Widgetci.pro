#-------------------------------------------------
#
# Project created by QtCreator 2017-06-21T19:13:09
#
#-------------------------------------------------

QT       += core gui qml quick

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Widgetci
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    wwidget.cpp \
    runguard.cpp

HEADERS  += mainwindow.h \
    wwidget.h \
    runguard.h

FORMS    += mainwindow.ui

QMAKE_CXXFLAGS += -std=c++11

RESOURCES += \
    qtresource.qrc
