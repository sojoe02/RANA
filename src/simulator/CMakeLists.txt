set(CORE_FILES
        core/agents/Agent.h
        core/agents/Agent.cpp
        core/agents/AgentLuaInterface.h
        core/agents/AgentLuaInterface.cpp
        event/EventQueue.h
        event/EventQueue.cpp
        core/FlowControl.h
        core/FlowControl.cpp
        core/Interfacer.h
        core/Interfacer.cpp
        core/Sector.h
        core/Sector.cpp
        core/Supervisor.h
        core/Supervisor.cpp
        )

set(API_FILES
        api/GridMovement.cpp
        api/GridMovement.h
        api/MapHandler.cpp
        api/MapHandler.h
        api/Phys.cpp
        api/Phys.h
        api/Scanning.h
        api/Scanning.cpp
        api/Shared.h
        api/Shared.cpp
        )

set(SIM_FILES
        control/Control.h
        control/Control.cpp
        utility/ID.h
        Output.h
        Output.cpp
        utility/Utility.h
        )

add_library(Simulator
        ${CORE_FILES}
        ${API_FILES}
        ${SIM_FILES}
        )

target_link_libraries(Simulator LuaIface)