#-------------------------------------------------
#
# Project created by QtCreator 2014-05-22T10:45:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Rana_qt
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    eventqueue.cpp \
    physics/phys.cpp \
    agentengine/agents/auton.cpp \
    agentengine/agents/autonlistener.cpp \
    agentengine/agents/autonLUA.cpp \
    agentengine/agents/autonscreamer.cpp \
    agentengine/agents/doctor.cpp \
    agentengine/agents/master.cpp \
    agentengine/agents/nestene.cpp \
    agentengine/agentdomain.cpp \
    output.cpp \
    physics/maphandler.cpp \
    control.cpp \
    graphics/agentItem.cpp \
    physics/gridmovement.cpp

HEADERS  += mainwindow.h \
    physics/phys.h \
    agentengine/agents/auton.h \
    agentengine/agents/autonlistener.h \
    agentengine/agents/autonLUA.h \
    agentengine/agents/autonscreamer.h \
    agentengine/agents/doctor.h \
    agentengine/agents/master.h \
    agentengine/agents/nestene.h \
    agentengine/agentdomain.h \
    eventqueue.h \
    ID.h \
    utility.h \
    output.h \
    physics/maphandler.h \
    control.h \
    graphics/agentItem.h \
    physics/gridmovement.h


FORMS    += mainwindow.ui

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += lua

QMAKE_CXXFLAGS += -std=c++11
