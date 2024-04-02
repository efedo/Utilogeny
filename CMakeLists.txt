# Copyright 2021-24 Eric Fedosejevs
#

cmake_minimum_required(VERSION 3.24)

include(CMakeDependentOption)

option(UTILOGENY_TBB "Build with TBB" ON)
option(UTILOGENY_GUI "Build Utilogeny GUI library" OFF)
option(UTILOGENY_PHYSICS "Build Utilogeny physics library" ON)
option(UTILOGENY_GTEST "Build with Google test" ON) # So that dependent packages can use Gtest without running Utilogeny test suite
option(UTILOGENY_TEST "Build with test support" ON)
option(UTILOGENY_AUDIO "Enable Utiligeny audio support." OFF)
option(UTILOGENY_SPDLOG "Enable Utiligeny spdlog support." ON)

set(OVERRIDE_CXX_STANDARD 20)

# These are temporary and will be replaced with find_package eventually
set(UTILOGENY_DIR "${CMAKE_CURRENT_SOURCE_DIR}" CACHE STRING "Path to Utilogeny")
include(${UTILOGENY_DIR}/cmake/setup_project.cmake)

pre_project_boilerplate(NAME "Utilogeny")

#return() # return here is required to keep Lifogeny from vomitting

project(Utilogeny VERSION 0.2
				  DESCRIPTION "Support library for RNA-see, Sirelphy, and Tenlaruen"
				  LANGUAGES CXX)
				  
post_project_boilerplate(NAME "Utilogeny")

add_subdirectory(source/core)
install(TARGETS UtilogenyCore DESTINATION lib)

#if(UTILOGENY_QT)
#	add_subdirectory(source/qt)
#	install(TARGETS Qtilogeny DESTINATION lib)
#endif()

if(UTILOGENY_GUI)
	add_subdirectory(source/gui)
	install(TARGETS UtilogenyGUI DESTINATION lib)
endif()

if(UTILOGENY_PHYSICS)
	add_subdirectory(source/physics)
	install(TARGETS UtilogenyPhysics DESTINATION lib)
endif()

if(UTILOGENY_TEST)
	if(NOT UTILOGENY_GTEST)
		message(FATAL_ERROR "Utilogeny tests require Google test (must set UTILOGENY_GTEST)")
	endif()
	add_subdirectory(source/test)
	install(TARGETS UtilogenyTest DESTINATION bin)
endif()


