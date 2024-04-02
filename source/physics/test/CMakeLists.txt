cmake_minimum_required(VERSION 3.10)

#include(${UTILOGENY_DIR}/cmake/target_add_googletest.cmake)
#include(${UTILOGENY_DIR}/cmake/target_add_tbb.cmake)

set(OVERRIDE_CXX_STANDARD 20)

add_executable(UtilogenyPhysicsTest
	pch.cpp
	pch.h
	test.cpp
    vector.cpp
	)

target_precompile_headers(UtilogenyPhysicsTest
	PRIVATE pch.h)
	
target_config_compiler(UtilogenyPhysicsTest)

set_target_properties(UtilogenyPhysicsTest PROPERTIES
            CXX_STANDARD ${OVERRIDE_CXX_STANDARD}
            CXX_EXTENSIONS OFF
)

target_link_libraries(UtilogenyPhysicsTest PUBLIC UtilogenyPhysics PRIVATE UtilogenyCore)

