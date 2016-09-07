#-------------------------------------------------
#
# Project created by QtCreator 2014-05-22T10:45:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += warn_on
TARGET = Rana_qt
TEMPLATE = app




SOURCES += main.cpp\
        mainwindow.cpp \
    output.cpp \
    control.cpp \
    graphics/agentItem.cpp \
    runner.cpp \
    postprocessing/eventprocessing.cpp \
    postprocessing/eventmap.cpp \
    postprocessing/colorutility.cpp \
    eventdialog.cpp \
    postprocessing/graphics/zblock.cpp \
    postprocessing/postcontrol.cpp \
    postprocessing/eventrunner.cpp \
    postprocessing/graphics/zmap.cpp \
    helpdialog.cpp \
    api/gridmovement.cpp \
    api/maphandler.cpp \
    api/phys.cpp \
    api/scanning.cpp \
    api/shared.cpp \
    simulationcore/flowcontrol.cpp \
    simulationcore/interfacer.cpp \
    simulationcore/sector.cpp \
    simulationcore/supervisor.cpp \
    simulationcore/agents/agent.cpp \
    simulationcore/agents/agentluainterface.cpp \
    simulationcore/eventqueue.cpp

HEADERS  += mainwindow.h \
    ID.h \
    utility.h \
    output.h \
    control.h \
    graphics/agentItem.h \
    runner.h \
    postprocessing/eventprocessing.h \
    postprocessing/eventmap.h \
    postprocessing/colorutility.h \
    eventdialog.h \
    postprocessing/graphics/zblock.h \
    postprocessing/postcontrol.h \
    postprocessing/eventrunner.h \
    postprocessing/graphics/zmap.h \
    helpdialog.h \
    api/gridmovement.h \
    api/maphandler.h \
    api/phys.h \
    api/scanning.h \
    api/shared.h \
    simulationcore/flowcontrol.h \
    simulationcore/interfacer.h \
    simulationcore/sector.h \
    simulationcore/supervisor.h \
    simulationcore/agents/agent.h \
    simulationcore/agents/agentluainterface.h \
    simulationcore/eventqueue.h

FORMS    += mainwindow.ui \
	eventdialog.ui \
    about.ui

#copy the lua modules to the correct directory, "wherever" Qmake puts it
unix: copydata.commands = $(COPY_DIR) $$PWD/modules $$OUT_PWD
unix: first.depends = $(first) copydata
unix: export(first.depends)
unix: export(copydata.commands)
unix: QMAKE_EXTRA_TARGETS += first copydata
#
QMAKE_CXXFLAGS += -Wno-unused-variable -Wno-unused-parameter
CONFIG += c++14
#QMAKE_CXXFLAGS += -std=c++1y -Wno-unused-variable -Wno-unused-parameter
#CONFIG += stdlib=libc++ lc++abi o3

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += luajit
#unix: PKGCONFIG += lua51
#unix: PKGCONFIG += lua5.3



macx: QMAKE_CXXFLAGS += -std=c++14 -mmacosx-version-min=10.7

macx: LIBS += -stdlib=libc++ -mmacosx-version-min=10.7
macx: LIBS += -L$$PWD/../lua-5.2_MacOS107_lib/ -llua52

macx: INCLUDEPATH += $$PWD/../lua-5.2_MacOS107_lib/include
macx: DEPENDPATH += $$PWD/../lua-5.2_MacOS107_lib/include

macx: PRE_TARGETDEPS += $$PWD/../lua-5.2_MacOS107_lib/liblua52.a

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../lua51_64bit/ -llua5.1
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../lua51_64bit/ -llua5.1

INCLUDEPATH += $$PWD/../lua51_64bit/include
DEPENDPATH += $$PWD/../lua51_64bit/include

RESOURCES += \
    images.qrc
