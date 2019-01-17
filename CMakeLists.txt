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
        "${PROJECT_SOURCE_DIR}/src"
        "${PROJECT_SOURCE_DIR}/dependencies/asio/asio/include"
        #        simulator"
        #        "${PROJECT_SOURCE_DIR}/dependencies"
        #        "${PROJECT_SOURCE_DIR}/service"
        )

add_compile_definitions(ASIO_STANDALONE)

#### Subdirectories for local libraries ####
add_subdirectory(src/simulator)
add_subdirectory(src/service)
add_subdirectory(src/control)
add_subdirectory(src/observation)
add_subdirectory(dependencies)
add_subdirectory(test)

set(CMAKE_CXX_FLAGS "-O3 -m64")
set(CMAKE_CXX_STANDARD 14)

#### Handle the executables and their linking libraries ####
add_executable(${PROJECT_NAME} "main.cpp")
target_link_libraries(${PROJECT_NAME}
        PUBLIC Simulator
        PUBLIC Service
        PRIVATE LuaIface
        PUBLIC ${CMAKE_THREAD_LIBS_INIT}
        )

add_executable(TEST_TCP_CLIENT "src/service/TestClient.cpp")
target_link_libraries(TEST_TCP_CLIENT
        PUBLIC ${CMAKE_THREAD_LIBS_INIT}
        )

add_executable(RUNTIME_TESTING "test/runtime/RunTimeTesting.cpp")
target_link_libraries(RUNTIME_TESTING
        PUBLIC Simulator
        PUBLIC Service
        PUBLIC ${CMAKE_THREAD_LIBS_INIT}
        )


