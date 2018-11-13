set(SERVICE_FILES
        ranaservice.cpp
        ranaservice.h
        )

add_library(Service
        ${SERVICE_FILES}
        )

target_link_libraries(Service Simulator)