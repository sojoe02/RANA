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

SOURCES += src/main.cpp\
    src/cli.cpp \
    src/mainwindow.cpp \
    src/parser.cpp \
    src/output.cpp \
    src/control.cpp \
    src/graphics/agentItem.cpp \
    src/runner.cpp \
    src/postprocessing/eventprocessing.cpp \
    src/postprocessing/colorutility.cpp \
    src/eventdialog.cpp \
    src/postprocessing/graphics/zblock.cpp \
    src/postprocessing/postcontrol.cpp \
    src/postprocessing/eventrunner.cpp \
    src/postprocessing/graphics/zmap.cpp \
    src/helpdialog.cpp \
    src/api/gridmovement.cpp \
    src/api/maphandler.cpp \
    src/api/phys.cpp \
    src/api/scanning.cpp \
    src/api/shared.cpp \
    src/simulationcore/flowcontrol.cpp \
    src/simulationcore/interfacer.cpp \
    src/simulationcore/sector.cpp \
    src/simulationcore/supervisor.cpp \
    src/simulationcore/agents/agent.cpp \
    src/simulationcore/agents/agentluainterface.cpp \
    src/simulationcore/eventqueue.cpp \
    src/api/tcpserver.cpp \
    src/api/tcpclient.cpp

HEADERS += src/cli.h \
    src/mainwindow.h \
    src/parser.h \
    src/ID.h \
    src/utility.h \
    src/output.h \
    src/control.h \
    src/graphics/agentItem.h \
    src/runner.h \
    src/postprocessing/eventprocessing.h \
    src/postprocessing/colorutility.h \
    src/eventdialog.h \
    src/postprocessing/graphics/zblock.h \
    src/postprocessing/postcontrol.h \
    src/postprocessing/eventrunner.h \
    src/postprocessing/graphics/zmap.h \
    src/helpdialog.h \
    src/api/gridmovement.h \
    src/api/maphandler.h \
    src/api/phys.h \
    src/api/scanning.h \
    src/api/shared.h \
    src/simulationcore/flowcontrol.h \
    src/simulationcore/interfacer.h \
    src/simulationcore/sector.h \
    src/simulationcore/supervisor.h \
    src/simulationcore/agents/agent.h \
    src/simulationcore/agents/agentluainterface.h \
    src/simulationcore/eventqueue.h \
    src/api/tcpclient.h \
    src/api/tcpserver.h

FORMS   += ui/mainwindow.ui \
    ui/eventdialog.ui \
    ui/about.ui

#copy the lua modules to the correct directory, "wherever" Qmake puts it
unix: copydata.commands = $(COPY_DIR) $$PWD/src/modules $$OUT_PWD
unix: first.depends = $(first) copydata
unix: export(first.depends)
unix: export(copydata.commands)
unix: QMAKE_EXTRA_TARGETS += first copydata
#
QMAKE_CXXFLAGS += -Wextra -Wno-unused-variable -Wno-unused-parameter
QMAKE_CXXFLAGS_RELEASE += -o3
CONFIG += c++14
#QMAKE_CXXFLAGS += -std=c++1y -Wno-unused-variable -Wno-unused-parameter
#CONFIG += stdlib=libc++ lc++abi o3

unix: CONFIG += link_pkgconfig
#unix: PKGCONFIG += luajit
unix: PKGCONFIG += lua51
#unix: PKGCONFIG += lua5.3

unix: RCC_DIR = ./BUILD/rcc/
unix: OBJECTS_DIR = ./BUILD/obj/
unix: MOC_DIR = ./BUILD/moc/
unix: UI_DIR = ./BUILD/ui/

unix: LIBS += -largtable2

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
    resources/images.qrc

DISTFILES += \
    src/lua_agents/01_pingpong.lua \
    src/lua_agents/01_pingpong_01.lua \
    src/lua_agents/01_pingpong_02.lua \
    src/lua_agents/01_pingpong_03.lua \
    src/lua_agents/02_data_collector.lua \
    src/lua_agents/02_master.lua \
    src/lua_agents/02_oscillator.lua \
    src/lua_agents/03_painter.lua \
    src/lua_agents/04_valueShare.lua \
    src/lua_agents/05_angular_event_module.lua \
    src/lua_agents/05_bat.lua \
    src/lua_agents/05_master.lua \
    src/lua_agents/05_pole.lua \
    src/lua_agents/06_mover.lua \
    src/lua_agents/07_repulser.lua \
    src/lua_agents/08_flasher.lua \
    src/lua_agents/08_master_flasher.lua \
    src/lua_agents/09_radial_scanner.lua \
    src/lua_agents/10_pingpong_targeted.lua \
    src/lua_agents/11_data_collector.lua \
    src/lua_agents/11_forage_module.lua \
    src/lua_agents/11_frog.lua \
    src/lua_agents/11_master.lua \
    src/lua_agents/11_oscillator_module.lua \
    src/lua_agents/12_master.lua \
    src/lua_agents/13_precision_test.lua \
    src/lua_agents/15_master.lua \
    src/lua_agents/15_female.lua \
    src/lua_agents/15_freerunning_osc.lua \
    src/lua_agents/15_greenfield_osc.lua \
    src/modules/auxiliary.lua \
    src/modules/lib_env_lake.lua \
    src/modules/lib_table.lua \
    src/modules/ranalib_agent.lua \
    src/modules/ranalib_api.lua \
    src/modules/ranalib_collision.lua \
    src/modules/ranalib_core.lua \
    src/modules/ranalib_draw.lua \
    src/modules/ranalib_environment.lua \
    src/modules/ranalib_event.lua \
    src/modules/ranalib_map.lua \
    src/modules/ranalib_movement.lua \
    src/modules/ranalib_shared.lua \
    src/modules/ranalib_statistic.lua \
    src/modules/ranalib_utility.lua \
    src/modules/ranalib_math.lua \
    src/modules/ranalib_physics.lua \
    src/modules/wrapper_auxiliary.lua \
    src/modules/ranalib_variable.lua \
    src/modules/test_file_back.lua \
    src/modules/test_file_input.lua \
    src/modules/ranalib_simconfig.lua \
    src/lua_agents/16_neuron.lua


