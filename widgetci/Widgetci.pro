#-------------------------------------------------
#
# Project created by QtCreator 2017-06-21T19:13:09
# Author: Mehmet Emin Fedar
#
#-------------------------------------------------

QT += widgets quick

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

!win32 {QMAKE_CXXFLAGS += -std=c++11}

RESOURCES += \
    qtresource.qrc
