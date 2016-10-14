#-------------------------------------------------
#
# Project created by QtCreator 2016-10-07T23:08:08
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LMD_processor

TEMPLATE = app

SOURCES += main.cpp\
           mainwindow.cpp \
           cls_LmdFile.cpp \
           external/LmdFile.cxx \
           external/fLmd.c \
           support.cpp \
           external/MbsTypeDefs.cxx \
           data/cls_Event.cpp \
           Go4QtRoot/QRootApplication.cpp \
           Go4QtRoot/QRootCanvas.cpp \
           Go4QtRoot/QRootDialog.cpp \
           Go4QtRoot/QRootWindow.cpp \
           gui/GraphWidget.cpp \
           gui/HistoWidget.cpp \
           data/cls_RootEvent.cpp \
           data/cls_RootHit.cpp \
    ../build-LMD_processor-Desktop-Debug/eventsdict.cxx \
    cls_Calibrator.cpp

HEADERS += mainwindow.h \
           cls_LmdFile.h \
           external/LmdFile.h \
           external/MbsTypeDefs.h \
           external/LmdTypeDefs.h \
           external/sMbs.h \
           external/fLmd.h \
           support.h \
           data/cls_Event.h \
           Go4QtRoot/QRootApplication.h \
           Go4QtRoot/QRootCanvas.h \
           Go4QtRoot/QRootDialog.h \
           Go4QtRoot/QRootWindow.h \
           Go4QtRoot/TGo4LockGuard.h \
           gui/GraphWidget.h \
           gui/HistoWidget.h \
           data/cls_RootEvent.h \
           data/cls_RootHit.h \
    cls_Calibrator.h

FORMS   += mainwindow.ui \
           gui/GraphWidget.ui \
           gui/HistoWidget.ui

# Set correct path to your ROOT include directory

INCLUDEPATH += /usr/include/root/ \
    Go4QtRoot/ \
    data/ \
    gui/

# Set correct path to your ROOT libs directory

LIBS += -lX11 \
        -L/usr/lib/x86_64-linux-gnu/ \
        -lGui -lCore -lRIO -lNet -lHist -lGraf -lGraf3d \
        -lGpad -lTree -lRint -lPostscript -lMatrix \
        -lPhysics -lMathCore -lThread -lCint \
        -lGed -lTreePlayer \
        -pthread -lm -ldl -rdynamic

# Use also -lCint for ROOT5

QMAKE_CXXFLAGS += -std=c++11 -Wall -Wextra -Wpedantic

DISTFILES += \
    drawTwoHistos.rootmacro
