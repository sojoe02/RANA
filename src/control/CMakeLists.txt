set(CONTROL_FILES
        Control.h
        Control.cpp
        command/CommandInterface.cpp
        command/CommandInterface.h
        command/CommandLuaInterface.cpp
        command/CommandLuaInterface.h
        )

add_library(Control ${CONTROL_FILES})

target_link_libraries(Control Simulator Globals Observation LuaIface)