#-------------------------------------------------
#
# Project created by QtCreator 2017-06-21T19:13:09
# Author: Mehmet Emin Fedar
#
#-------------------------------------------------

QT += widgets quick qml

TARGET = Widgetci
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    wwidget.cpp \
    runguard.cpp \
    wqmlfile.cpp \
    wqmlsystem.cpp

HEADERS  += mainwindow.h \
    wwidget.h \
    runguard.h \
    wqmlfile.h \
    wqmlsystem.h

FORMS    += mainwindow.ui

!win32 { QMAKE_CXXFLAGS += -std=c++11 }

win32{ CONFIG += static }

RESOURCES += \
    qtresource.qrc

RC_FILE = widgetci.rc
