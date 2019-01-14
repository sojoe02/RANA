set(TEST_FILES
        runtime/service/ServiceTest.cpp
        runtime/service/ServiceTest.h
        runtime/RunTimeTesting.cpp
        runtime/RunTimeTesting.h
        )

add_library(Test
        ${TEST_FILES}
        )

target_link_libraries(Test Simulator Service)