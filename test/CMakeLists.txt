set(TEST_FILES
        ServiceTest.cpp
        ServiceTest.h
        )

add_library(Test
        ${TEST_FILES}
        )

target_link_libraries(Simulator)