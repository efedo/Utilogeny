# Copyright 2021 Eric Fedosejevs
#

include(${UTILOGENY_DIR}/cmake/find_install_package.cmake)
include(${UTILOGENY_DIR}/cmake/target_deploy_lib.cmake)

# These need to be macros unfortunately because Qt sets various global variables required
# by automoc/rcc/ui
macro(target_include_copperspice)

    cmake_parse_arguments(
        PARSED_ARGS # prefix of output variables
        "" # list of names of the boolean arguments (only defined ones will be true)
        "TARGET" # list of names of mono-valued arguments
        "COMPONENTS" # list of names of multi-valued arguments (output variables are lists)
        ${ARGN} # arguments of the function to parse, here we take the all original ones
    )
	
	if((NOT PARSED_ARGS_TARGET) OR (NOT PARSED_ARGS_COMPONENTS))
		message(FATAL_ERROR "target_include_copperspice() or target_link_copperspice() not supplied valid arguments")
	endif()
	
	unset(COPPERSPICE_REQUIRED_TARGETS)
	foreach(REQUIRED_COMPONENT ${PARSED_ARGS_COMPONENTS})
		list(APPEND COPPERSPICE_REQUIRED_TARGETS "CopperSpice::${REQUIRED_COMPONENT}")
	endforeach()
	
	set(CopperSpice_DIR "${FIND_HINTS_COPPERSPICE}") # CopperSpice config requires this to be set

	find_install_package(
		PACKAGE_NAME "CopperSpice"
		COMPONENTS "${PARSED_ARGS_COMPONENTS}"
		REQUIRED_TARGETS "${COPPERSPICE_REQUIRED_TARGETS}"
		SEARCH_HINTS "${FIND_HINTS_COPPERSPICE}"
		#GIT_REPOSITORY "https://github.com/copperspice/copperspice"
	)
	
	# if(NOT TARGET CopperSpice::CsCore)
		# set(WITH_GUI ON)
		# set(WITH_OPENGL ON)
		# set(WITH_MULTIMEDIA OFF)
		# set(WITH_WEBKIT OFF)
		# set(WITH_SVG OFF)
		# set(WITH_SQL OFF)
		# set(WITH_XML OFF)
		# set(WITH_XMLPATTERNS OFF)
		# set(WITH_NETWORK OFF)
		# set(WITH_SCRIPT OFF)
		# add_subdirectory("C:/Programming/lib/copperspice" "${CMAKE_BINARY_DIR}/lib/CopperSpice")
	# endif()
	
	target_include_lib(LINK_TARGET ${PARSED_ARGS_TARGET} PACKAGE_NAME "CopperSpice" DEBUG_MESSAGES)		
	
	foreach(LIBRARY_REQUIRED ${PARSED_ARGS_COMPONENTS})
		message(STATUS "Target: ${PARSED_ARGS_TARGET} Link target: CopperSpice::${LIBRARY_REQUIRED}")
		target_include_lib(LINK_TARGET ${PARSED_ARGS_TARGET} PACKAGE_TARGETS "CopperSpice::${LIBRARY_REQUIRED}" DEBUG_MESSAGES)			
	endforeach()
endmacro()

macro(target_link_copperspice)
    cmake_parse_arguments(
        PARSED_ARGS # prefix of output variables
        "" # list of names of the boolean arguments (only defined ones will be true)
        "TARGET" # list of names of mono-valued arguments
        "COMPONENTS" # list of names of multi-valued arguments (output variables are lists)
        ${ARGN} # arguments of the function to parse, here we take the all original ones
    )
	
	foreach(REQUIRED_COMPONENT ${PARSED_ARGS_COMPONENTS})
		list(APPEND COPPERSPICE_TARGETS "CopperSpice::${REQUIRED_COMPONENT}")
	endforeach()
	
	target_include_copperspice(TARGET ${PARSED_ARGS_TARGET} COMPONENTS ${PARSED_ARGS_COMPONENTS})
	target_link_deploy_lib(LINK_TARGET ${PARSED_ARGS_TARGET} PACKAGE_NAME "CopperSpice")
	
	# Install required DLLs	
	message(STATUS "Deploying CopperSpice manually.")
	foreach(LIBRARY_REQUIRED ${PARSED_ARGS_COMPONENTS})
		message(STATUS "Target: ${PARSED_ARGS_TARGET} Link target: CopperSpice::${LIBRARY_REQUIRED}")
		target_link_deploy_lib(LINK_TARGET ${PARSED_ARGS_TARGET} PACKAGE_TARGETS "CopperSpice::${LIBRARY_REQUIRED}")			
	endforeach()
	
	# see cs_copy_plugins from CopperSpiceDeploy.cmake (which does not copy to build dir)
	file(GLOB COPPERSPICE_PLATFORM_LIBS 
		"${CS_PLUGIN_DIR}/CsGui*.dll"
		"${CS_PLUGIN_DIR}/CsGui*.so*"
	)
	
	foreach(PLATFORM_LIB ${COPPERSPICE_PLATFORM_LIBS})
		get_filename_component(LIB_NAME "${PLATFORM_LIB}" NAME)
		set(LIB_OUTPUT_PATH "${CMAKE_CURRENT_BINARY_DIR}/platforms/${LIB_NAME}")
		message(STATUS "CopperSpice platform GUI lib ${PLATFORM_LIB} being installed to ${LIB_OUTPUT_PATH}")
		add_custom_command(TARGET "${PARSED_ARGS_TARGET}" COMMAND ${CMAKE_COMMAND} -E copy_if_different "${PLATFORM_LIB}" "${LIB_OUTPUT_PATH}")
		install(FILES "${PLATFORM_LIB}" DESTINATION bin/platforms)
	endforeach()
	
	file(GLOB COPPERSPICE_X_LIB 
		"${CS_INSTALLED_LIB_DIR}/libCsXcbSupport*.so"
	)
	
	if(COPPERSPICE_X_LIB)
		get_filename_component(X_LIB_NAME "${COPPERSPICE_X_LIB}" NAME)
		set(X_LIB_OUTPUT_PATH "${CMAKE_CURRENT_BINARY_DIR}/platform/${X_LIB_NAME}")
		message(STATUS "CopperSpice X lib ${COPPERSPICE_X_LIB} being installed to ${X_LIB_OUTPUT_PATH}")
		add_custom_command(TARGET "${PARSED_ARGS_TARGET}" COMMAND ${CMAKE_COMMAND} -E copy_if_different "${COPPERSPICE_X_LIB}" "${X_LIB_OUTPUT_PATH}")
		install(FILES "${COPPERSPICE_X_LIB}" DESTINATION bin)
	endif()
endmacro()