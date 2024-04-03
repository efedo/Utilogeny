cmake_minimum_required(VERSION 3.25)

add_executable(UtilogenyPhysicsTest
	precomp.cpp
	precomp.h
	test.cpp
    vector.cpp
	)

target_precompile_headers(UtilogenyPhysicsTest
	PRIVATE precomp.h)
	
target_config_compiler(UtilogenyPhysicsTest)

set_target_properties(UtilogenyPhysicsTest PROPERTIES
            CXX_STANDARD ${OVERRIDE_CXX_STANDARD}
            CXX_EXTENSIONS OFF
)

target_link_libraries(UtilogenyPhysicsTest PUBLIC UtilogenyPhysics PRIVATE UtilogenyCore)

