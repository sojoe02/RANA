set(SERVICE_FILES
        ranaservice.cpp
        ranaservice.h
        ranainstance.cpp
        ranainstance.h
        testclient.cpp
        testclient.h
        testserver.cpp
        testserver.h
        )

add_library(Service
        ${SERVICE_FILES}
        )

target_link_libraries(Service Simulator)