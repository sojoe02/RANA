set(GLOBALS_FILES
        RanaDefinitions.hpp
        )

add_library(Globals ${GLOBALS_FILES})

set_target_properties(Globals PROPERTIES LINKER_LANGUAGE CXX)