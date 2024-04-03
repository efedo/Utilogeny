cmake_minimum_required(VERSION 3.25)

add_executable(UtilogenyGuiTest
	precomp.cpp
	precomp.h
	test.cpp
	)

target_precompile_headers(UtilogenyGuiTest
	PRIVATE precomp.h)
	
target_config_compiler(UtilogenyGuiTest)

set_target_properties(UtilogenyGuiTest PROPERTIES
            CXX_STANDARD ${OVERRIDE_CXX_STANDARD}
            CXX_EXTENSIONS OFF
)

target_link_libraries(UtilogenyGuiTest PUBLIC UtilogenyGui PRIVATE UtilogenyCore)

