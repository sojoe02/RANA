cmake_minimum_required(VERSION 2.8)
project(Rana)

set(Rana_VERSION_MAJOR 2)
set(Rana_VERSION_MINOR 0)
set(Rana_VERSION_FIX 0)

find_package(Threads)

configure_file(
        ${PROJECT_SOURCE_DIR}/ranaconfig.h.in
        ${PROJECT_BINARY_DIR}/ranaconfig.h
)

#### include directories ####
INCLUDE_DIRECTORIES("${PROJECT_BINARY_DIR}"
        "${PROJECT_SOURCE_DIR}"
        #        simulator"
        #        "${PROJECT_SOURCE_DIR}/dependencies"
        #        "${PROJECT_SOURCE_DIR}/webservice"
        )


#### Subdirectories for local libraries ####
add_subdirectory(simulator)
#add_subdirectory(webservice)
add_subdirectory(dependencies)

set(CMAKE_CXX_FLAGS "-O3 -m64")
set(CMAKE_CXX_STANDARD 14)

add_executable(${PROJECT_NAME} "main.cpp")

target_link_libraries(${PROJECT_NAME}
        PRIVATE Simulator
        #	    //Webservice
        PRIVATE LuaIface
        PUBLIC ${CMAKE_THREAD_LIBS_INIT}
        )
