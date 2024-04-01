# Copyright 2021 Eric Fedosejevs
#

cmake_minimum_required(VERSION 3.15)

if(UTILOGENY_AUDIO)
	include(${UTILOGENY_DIR}/cmake/target_add_SDL.cmake)
	include(${UTILOGENY_DIR}/cmake/target_add_soloud.cmake)
endif()

add_executable(UtilogenyTest
	#../core/properties/property_test.cpp
	pch.cpp
	pch.h
	test.cpp
)

target_precompile_headers(UtilogenyTest
	PRIVATE pch.h)
	
target_config_compiler(UtilogenyTest)

set_target_properties(UtilogenyTest PROPERTIES
            CXX_STANDARD ${OVERRIDE_CXX_STANDARD}
            CXX_EXTENSIONS OFF
            )	

if(UTILOGENY_AUDIO)
	target_link_soloud(UtilogenyTest)
	target_link_sdl2(UtilogenyTest)
endif()

target_link_libraries(UtilogenyTest PUBLIC UtilogenyCore)