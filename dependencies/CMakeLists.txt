######################################################################
#### Scripting language Lua                                       ####
######################################################################
set(LuaPath "lua-5.3.5")

add_custom_target(LUA_LIB
        COMMAND make linux
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/dependencies/${LuaPath}"
        )

add_library(LuaIface
        luascripting.h
        luascripting.cpp
        )

target_link_libraries(LuaIface
        ${PROJECT_SOURCE_DIR}/dependencies/${LuaPath}/src/liblua.a
        )

add_dependencies(LuaIface LUA_LIB)

target_include_directories(LuaIface PUBLIC
        "${LuaPath}/src"
        )
