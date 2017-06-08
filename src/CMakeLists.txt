set( GENERAL
	main.cpp
	)

set( INTERFACE
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
	)

set(CORE
	ID.h
	)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/interface
	${CMAKE_CURRENT_SOURCE_DIR}/environment
	${CMAKE_CURRENT_SOURCE_DIR}/api
	#${CMAKE_CURRENT_SOURCE_DIR}/libraries/mtrand
)


add_executable(${PROJECT_NAME}
	${GENERAL}
	${API}
	${ENVIRONMENT}
	${CORE}
	${INTERFACE}
	${version_file}
	)

link_directories(
	 ${LIBZMQ_LIBRARY_DIRS}
 )
 include_directories(
	 ${LIBZMQ_INCLUDE_DIRS}
 )

target_link_libraries(${PROJECT_NAME} ${LIBZMQ_LIBRARIES})
