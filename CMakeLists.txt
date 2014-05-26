#------------------------------------------------
#Add all executables:
#------------------------------------------------
#Agent Engine:
set (AGENTENGINE
	agentengine/agentdomain.cpp
	agentengine/agentdomain.h
	agentengine/agents/auton.cpp
	agentengine/agents/auton.h
	agentengine/agents/autonlistener.cpp
	agentengine/agents/autonlistener.h
	agentengine/agents/autonscreamer.cpp
	agentengine/agents/autonscreamer.h
	agentengine/agents/autonLUA.cpp
	agentengine/agents/autonLUA.h
	agentengine/agents/doctor.cpp
	agentengine/agents/doctor.h
	agentengine/agents/master.cpp
	agentengine/agents/master.h
	agentengine/agents/nestene.cpp
	agentengine/agents/nestene.h
)
#Physics:
set (PHYSICS
	physics/phys.cpp
	physics/phys.h
)
#General files:
set (GENERAL
	eventqueue.cpp
	eventqueue.h
	ID.h
	utility.h
	main.cpp
	output.cpp
	output.h	
)
#------------------------------------------------
#Set subdirectories:
#------------------------------------------------
include_directories(${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/agentengine
	${CMAKE_CURRENT_SOURCE_DIR}/agentengine/agents	
	${CMAKE_CURRENT_SOURCE_DIR}/physics
	#${CMAKE_CURRENT_SOURCE_DIR}/libraries/mtrand
)

add_executable(kasterborous ${AGENTENGINE} ${PHYSICS} ${MTRAND} ${GENERAL})
#------------------------------------------------
#Handle LUA implementation:
#------------------------------------------------
if(LUA52_FOUND)
	include_directories(${LUA_INCLUDE_DIR})
	target_link_libraries(kasterborous ${LUA_LIBRARY})
endif(LUA52_FOUND)
#------------------------------------------------
#Handle NCurses:
#------------------------------------------------
if(CURSES_FOUND)
	include_directories(${CURSES_INCLUDE_DIR})
	target_link_libraries(kasterborous ${CURSES_LIBRARIES} )
endif(CURSES_FOUND)

# add the install targets
install(TARGETS kasterborous DESTINATION ${PROJECT_SOURCE_DIR}/bin)
install (FILES "${PROJECT_BINARY_DIR}/kasterborous.h"        
	DESTINATION ${PROJECT_SOURCE_DIR}/bin/include)

set(CMAKE_CXX_FLAGS "-g -pthread -std=c++11")

#"-pg -pthread -std=c++11")
