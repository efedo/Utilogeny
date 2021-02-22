# Copyright 2021 Eric Fedosejevs
#

cmake_minimum_required(VERSION 3.10)

include(${UTILOGENY_DIR}/cmake/target_add_googletest.cmake)
include(${UTILOGENY_DIR}/cmake/target_add_SDL.cmake)
include(${UTILOGENY_DIR}/cmake/target_add_soloud.cmake)


add_executable(UtilogenyTest
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

target_link_soloud(UtilogenyTest)
target_link_tbb(UtilogenyTest)
target_link_sdl2(UtilogenyTest)
target_link_gtest(UtilogenyTest)

target_link_libraries(UtilogenyTest UtilogenyCore)

