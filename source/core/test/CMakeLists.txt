# Copyright 2021 Eric Fedosejevs
#

cmake_minimum_required(VERSION 3.25)

add_executable(UtilogenyCoreTest
	#../core/properties/property_test.cpp
	precomp.cpp
	precomp.h
	test.cpp
)

target_precompile_headers(UtilogenyCoreTest
	PRIVATE precomp.h)
	
target_config_compiler(UtilogenyCoreTest)

set_target_properties(UtilogenyCoreTest PROPERTIES
            CXX_STANDARD ${OVERRIDE_CXX_STANDARD}
            CXX_EXTENSIONS OFF
            )	

if(UTILOGENY_AUDIO)
	target_link_libraries(UtilogenyCoreTest PUBLIC soloud SDL2-static)
endif()

target_link_libraries(UtilogenyCoreTest PUBLIC UtilogenyCore)