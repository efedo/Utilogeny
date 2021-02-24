# Copyright 2021 Eric Fedosejevs
#

cmake_minimum_required(VERSION 3.15)

# Policies must be defined before project
# Defaults must be set as well to be inherited by subdirectories

cmake_policy(SET CMP0048 NEW)
set(CMAKE_POLICY_DEFAULT_CMP0048 NEW)
cmake_policy(SET CMP0077 NEW)
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
cmake_policy(SET CMP0069 NEW)
set(CMAKE_POLICY_DEFAULT_CMP0069 NEW)
cmake_policy(SET CMP0087 NEW)			# Allow use of generator expressions in install codes/scripts
set(CMAKE_POLICY_DEFAULT_CMP0087 NEW)

set(OVERRIDE_CXX_STANDARD "17" CACHE STRING "C++ standard")
set(CMAKE_CXX_STANDARD ${OVERRIDE_CXX_STANDARD})
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#message(STATUS "Set CXX_STANDARD to: ${OVERRIDE_CXX_STANDARD}")

macro(pre_project_boilerplate)
    cmake_parse_arguments(
        PARSED_ARGS # prefix of output variables
        "" # list of names of the boolean arguments (only defined ones will be true)
        "NAME" # list of names of mono-valued arguments
        "" # list of names of multi-valued arguments (output variables are lists)
        ${ARGN} # arguments of the function to parse, here we take the all original ones
    )
	
	if(NOT PARSED_ARGS_NAME)
		message(FATAL_ERROR "Missing project setup argument")
	endif()
	
	if(NOT UTILOGENY_DIR)
		message(FATAL_ERROR "Did not supply Utilogeny directory") # For now needs to be manually set
		#set(UTILOGENY_DIR CACHE STRING "${CMAKE_CURRENT_SOURCE_DIR}/../") #because this file should be in Utilogeny CMake folder
	endif()
	
	if(TARGET "${PARSED_ARGS_NAME}DummyTarget")
		return()
	else()
		add_custom_target("${PARSED_ARGS_NAME}DummyTarget")
	endif()
	
	string(TOUPPER ${PARSED_ARGS_NAME} CAPS_NAME)

	message(STATUS "Configuring project ${PARSED_ARGS_NAME} in ${CMAKE_CURRENT_SOURCE_DIR}")
	
	set(${CAPS_NAME}_DIR CACHE STRING "${CMAKE_CURRENT_SOURCE_DIR}")
		
	list(APPEND CMAKE_MODULE_PATH ${UTILOGENY_DIR}/cmake)
endmacro()

macro(post_project_boilerplate)

	if(NOT UTILOGENY_DIR)
		message(FATAL_ERROR "Did not supply Utilogeny directory") # For now needs to be manually set
		#set(UTILOGENY_DIR CACHE STRING "${CMAKE_CURRENT_SOURCE_DIR}/../") #because this file should be in Utilogeny CMake folder
	endif()
								  
	# Remove default RTTI
	string(REPLACE "/GR" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})				  

	include(${UTILOGENY_DIR}/cmake/optimization.cmake)
	include(${UTILOGENY_DIR}/cmake/target_config_compiler.cmake)

	include_directories("..") # All include paths will be relative to parent directory
endmacro()