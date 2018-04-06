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
           cls_Calibrator.cpp \
           cls_pixelMap.cpp \
           cls_CrossTalkAnalyser.cpp \
           ../build-LMD_processor-Desktop-Debug/eventsdict.cxx

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
           cls_Calibrator.h \
           cls_pixelMap.h \
           cls_CrossTalkAnalyser.h

FORMS   += mainwindow.ui \
           gui/GraphWidget.ui \
           gui/HistoWidget.ui

# Set correct path to your ROOT include directory

INCLUDEPATH += $$system("root-config --incdir") \
    Go4QtRoot/ \
    data/ \
    gui/

# Set correct path to your ROOT libs directory
# maybe add -lX11

LIBS += $$system("root-config --libs")

# Use also -lCint for ROOT5
# and delete -lMultiProc and -lCling at the same time

QMAKE_CXXFLAGS += -std=c++11 -Wall -Wextra -Wpedantic

DISTFILES += \
    drawTwoHistos.rootmacro
