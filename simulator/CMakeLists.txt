set(CORE_FILES
        core/agents/agent.h
        core/agents/agent.cpp
        core/agents/agentluainterface.h
        core/agents/agentluainterface.cpp
        event/eventqueue.h
        event/eventqueue.cpp
        core/flowcontrol.h
        core/flowcontrol.cpp
        core/interfacer.h
        core/interfacer.cpp
        core/sector.h
        core/sector.cpp
        core/supervisor.h
        core/supervisor.cpp
        )

set(API_FILES
        api/gridmovement.cpp
        api/gridmovement.h
        api/maphandler.cpp
        api/maphandler.h
        api/phys.cpp
        api/phys.h
        api/scanning.h
        api/scanning.cpp
        api/shared.h
        api/shared.cpp
        )

set(SIM_FILES
        control.h
        control.cpp
        utility/ID.h
        output.h
        output.cpp
        utility/utility.h
        )

add_library(Simulator
        ${CORE_FILES}
        ${API_FILES}
        ${SIM_FILES}
        )

target_link_libraries(Simulator LuaIface)