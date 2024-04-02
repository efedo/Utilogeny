# Copyright 2021-24 Eric Fedosejevs
#

cmake_minimum_required(VERSION 3.24)

#option(UTILOGENY_TEST "Build with test support" ON)

#set(UTILOGENY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../Utilogeny" CACHE STRING "Path to Utilogeny" FORCE)
#set(SIRELPHY_DIR "${CMAKE_CURRENT_SOURCE_DIR}" CACHE STRING "Path to Sirelphy" FORCE)

#include(${UTILOGENY_DIR}/cmake/setup_project.cmake)

#pre_project_boilerplate(NAME "Sirelphy")

#project(Sirelphy VERSION 1.0
#                  DESCRIPTION "May eventually become a SImulated RELativistic PHYsics library. Under very early development."
#                  LANGUAGES CXX)
				  
#post_project_boilerplate(NAME "Sirelphy")

#if(NOT TARGET UtilogenyCore)
#	if(SIRELPHY_TEST)
#		set(UTILOGENY_GTEST ON)
#		set(UTILOGENY_TEST OFF)
#	endif()
#	add_subdirectory(${UTILOGENY_DIR} ${CMAKE_BINARY_DIR}/lib/Utilogeny)
#endif()

add_subdirectory(core)
if(UTILOGENY_TEST)
	add_subdirectory(test)
endif()

install(TARGETS UtilogenyPhysics DESTINATION lib)
if(UTILOGENY_TEST)
	install(TARGETS UtilogenyPhysicsTest DESTINATION bin)
endif()