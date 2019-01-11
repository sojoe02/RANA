set(SERVICE_FILES
        RanaService.cpp
        RanaService.h
        RanaInstance.cpp
        RanaInstance.h
        TestClient.cpp
        TestClient.h
        TestServer.cpp
        TestServer.h
        )

add_library(Service
        ${SERVICE_FILES}
        )

target_link_libraries(Service Simulator)