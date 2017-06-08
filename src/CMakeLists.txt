set( GENERAL
	main.cpp
	inbound.h
	inbound.cpp
	output.h
	output.cpp
	)

add_executable(${PROJECT_NAME} ${GENERAL} ${version_file})
