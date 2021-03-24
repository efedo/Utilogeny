# Copyright 2021 Eric Fedosejevs
#

include(${UTILOGENY_DIR}/cmake/git_submodule.cmake)
include(${UTILOGENY_DIR}/cmake/find_install_package.cmake)
include(${UTILOGENY_DIR}/cmake/target_deploy_lib.cmake)
#include(${UTILOGENY_DIR}/cmake/target_add_sdl.cmake)

function(target_include_urho3d target)
	#find_install_package(FIND_PACKAGE_NAME "Soloud" VCPKG_NAME "none" CONAN_NAME "none" GIT_REPOSITORY "https://github.com/jarikomppa/soloud.git" POSSIBLE_TARGETS "GTest::gtest;GTest::gtest_main")
	#target_include_directories(${target} PUBLIC "${PROJECT_SOURCE_DIR}/lib/urho3d/include")
endfunction()

function(target_link_urho3d target)
	if(NOT TARGET Urho3D)
	
		message(FATAL_ERROR "Gave up on Urho3D inclusion, since its build system and CMake setup are a complete mess")
		
		set(Urho3D_DIR "${ABSTRACTORIZER_DIR}/lib/Urho3D")
		#set(URHO3D_HOME "${CMAKE_BINARY_DIR}/lib/Urho3D")
		
		# link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

		# set(URHO3D_INSTALL_DEBUG "${URHO3D_DIR}/out/install/x64-Debug")
		# set(URHO3D_INSTALL_RELEASE "${URHO3D_DIR}/out/install/x64-Release")
		# set(CMAKE_MODULE_PATH "${URHO3D_DIR}/CMake/Modules" ${CMAKE_MODULE_PATH})


		# if (${CurrTargetType} STREQUAL "release")
			# set(CMAKE_PREFIX_PATH ${URHO3D_INSTALL_RELEASE} ${CMAKE_PREFIX_PATH})
			# include_directories( ${URHO3D_INSTALL_RELEASE}/include )
			# message(STATUS "Adding release include directory: '${URHO3D_INSTALL_RELEASE}/include'")
			# set(URHO3D_HOME ${URHO3D_INSTALL_RELEASE})
		# else()
			# set(CMAKE_PREFIX_PATH ${URHO3D_INSTALL_DEBUG} ${CMAKE_PREFIX_PATH})
			# include_directories( ${URHO3D_INSTALL_DEBUG}/include )
			# message(STATUS "Adding debug include directory: '${URHO3D_INSTALL_DEBUG}/include'")
			# set(URHO3D_HOME ${URHO3D_INSTALL_DEBUG})
		# endif()
		
		if (COMMAND cmake_policy)
			# Libraries linked via full path no longer produce linker search paths
			cmake_policy (SET CMP0003 NEW)
			# INTERFACE_LINK_LIBRARIES defines the link interface
			cmake_policy (SET CMP0022 NEW)
			# Disallow use of the LOCATION target property - so we set to OLD as we still need it
			cmake_policy (SET CMP0026 OLD)
			# MACOSX_RPATH is enabled by default
			cmake_policy (SET CMP0042 NEW)
			# Honor the visibility properties for SHARED target types only
			cmake_policy (SET CMP0063 OLD)
		endif ()

		# Include UrhoCommon.cmake module after setting project name
		#include (UrhoCommon)
		
		# In a regex, outside of [] you need \\$
		# In a regex, inside of [] you do not need anything.  [$] is fine.
		# In a non-regex string, in front of a curly bracket you need \${
		# set(blah "whatever but \${do_not_evaluate}")
		# In a regex string, in front of a curly bracket you need \\\${
		# string(REGEX REPLACE
		  # ".*whatever but \\\${do not evaluate}"
		
		message(STATUS "Trying to fix Urho3D CMakeLists.txt file")
		file(READ "${PROJECT_SOURCE_DIR}/lib/urho3d/CMakeLists.txt" filedata)
		if(NOT filedata)
			message(FATAL_ERROR "Something went wrong reading file: ${PROJECT_SOURCE_DIR}/lib/urho3d/CMakeLists.txt")
		endif()
		
		# fix so that Urho3D can be nested in another project
		
		string(FIND "${filedata}" "set (CMAKE_MODULE_PATH \${CMAKE_SOURCE_DIR}/cmake/Modules)" URHO_NEEDS_FIXING)	
		if(URHO_NEEDS_FIXING) # stops infinite loop due to MSVC's 'smart' automatic cmake configuration
			string(REPLACE "set (CMAKE_MODULE_PATH \${CMAKE_SOURCE_DIR}/cmake/Modules)" "set (CMAKE_MODULE_PATH \${PROJECT_SOURCE_DIR}/cmake/Modules)" filedata "${filedata}")	
			#file(WRITE "${PROJECT_SOURCE_DIR}/lib/urho3d/CMakeLists.txt" "${filedata}")
		endif()
		
		# Urho3D setup
		
		##Build the damn thing
		# if(OS_WINDOWS)
			# execute_process(COMMAND "${Urho3D_DIR}/gradlew.bat" "${URHO3D_HOME}"
			    # OUTPUT_FILE "${URHO3D_HOME}/cmake_build_out.txt"
                # ERROR_FILE "${URHO3D_HOME}/cmake_build_out.txt"
			# )
		# else()
			# execute_process(COMMAND "${Urho3D_DIR}/gradlew" "${URHO3D_HOME}"
				# OUTPUT_FILE "${URHO3D_HOME}/cmake_build_out.txt"
                # ERROR_FILE "${URHO3D_HOME}/cmake_build_out.txt"
			# )
		# endif()
		
		# message("Should have tried to build Urho3D here")
		
		# Set CMake modules search path
		#set(CMAKE_MODULE_PATH ${Urho3D_DIR}/CMake/Modules ${CMAKE_MODULE_PATH})
		
		##Include Urho3D Cmake common module
		# include("${Urho3D_DIR}/CMake/Modules/UrhoCommon.cmake")
		
		# Define target name
		#set(TARGET_NAME AbstractorizerGUI)
		set(URHO3D_STATIC_RUNTIME)
		set(URHO3D_SAMPLES)
		set(URHO3D_OPENGL)
		set(ANDROID 0)
		set(URHO3D_LIB_TYPE STATIC)
		add_subdirectory("${PROJECT_SOURCE_DIR}/lib/urho3d" "${CMAKE_BINARY_DIR}/lib/urho3d")
		
		# Define source files
		#define_source_files ()
		
		# Setup target with resource copying
		#setup_main_executable ()
		
		#add_subdirectory("${PROJECT_SOURCE_DIR}/lib/urho3d" "${CMAKE_BINARY_DIR}/lib/urho3d")
		#target_include_sdl2(soloud)
	endif()
	target_include_urho3d(${target})
	target_link_deploy_lib(LINK_TARGET ${target} PACKAGE_TARGETS "Urho3D")
endfunction()