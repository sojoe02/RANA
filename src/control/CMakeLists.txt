set(CONTROL_FILES
        Control.h
        Control.cpp
        CommandInterface.cpp
        CommandInterface.h
        )

add_library(Control ${CONTROL_FILES})

target_link_libraries(Control Simulator LuaIface)