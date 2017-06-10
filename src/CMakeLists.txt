set(GENERAL
		main.cpp
		ID.h
		utility.h
		)

set(INTERFACE
		interface/communication/inbound.h
		interface/communication/inbound.cpp
		interface/communication/outbound.h
		interface/communication/outbound.cpp
		)

set(ENVIRONMENT
		environment/environment.h
		environment/environment.cpp
		)

set(API
		api/phys.h
		api/phys.cpp
                api/shared.h
                api/shared.cpp
		)

set(CORE
		simulationcore/eventqueue.cpp
		simulationcore/eventqueue.h
		simulationcore/flowcontrol.h
		simulationcore/flowcontrol.cpp
		simulationcore/interfacer.cpp
		simulationcore/interfacer.h
		simulationcore/sector.cpp
		simulationcore/sector.h
		simulationcore/supervisor.cpp
		simulationcore/supervisor.h
		)

set(AGENTS
		simulationcore/agents/agent.cpp
		simulationcore/agents/agent.h
		simulationcore/agents/agentluainterface.cpp
		simulationcore/agents/agentluainterface.h
		)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}
		${CMAKE_CURRENT_SOURCE_DIR}/interface
		${CMAKE_CURRENT_SOURCE_DIR}/environment
		${CMAKE_CURRENT_SOURCE_DIR}/api
		)


add_executable(${PROJECT_NAME}
		${GENERAL}
		${API}
		${ENVIRONMENT}
		${CORE}
		${INTERFACE}
		${AGENTS}
		${version_file}
		)

link_directories(
		${LIBZMQ_LIBRARY_DIRS}
		${LUAJIT_LIBRARY_DIRS}

)
include_directories(
		${LIBZMQ_INCLUDE_DIRS}
		${LUAJIT_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} ${LIBZMQ_LIBRARIES} ${LUAJIT_LIBRARIES})
