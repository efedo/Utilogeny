# Copyright 2021-24 Eric Fedosejevs
#

cmake_minimum_required(VERSION 3.25)

add_executable(UtilogenyGuiExample1
	example1.cpp
	)

#target_precompile_headers(UtilogenyExample1
#	PRIVATE precomp.h)
	
target_config_compiler(UtilogenyGuiExample1)

set_target_properties(UtilogenyGuiExample1 PROPERTIES
            CXX_STANDARD ${OVERRIDE_CXX_STANDARD}
            CXX_EXTENSIONS OFF
)

target_link_libraries(UtilogenyGuiExample1 PUBLIC UtilogenyGui PRIVATE UtilogenyCore)

