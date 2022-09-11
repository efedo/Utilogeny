# Copyright 2021 Eric Fedosejevs
#

macro(check_include_link_args)
	#message(STATUS "PARSED_ARGS_PACKAGE_NAME: ${PARSED_ARGS_PACKAGE_NAME}")
	#message(STATUS "PARSED_ARGS_PACKAGE_TARGETS: ${PARSED_ARGS_PACKAGE_TARGETS}")

	if(NOT PARSED_ARGS_LINK_TARGET) # Need to specify a link target
		message(FATAL_ERROR "target_link_deploy or target_include not supplied target by ${CMAKE_CURRENT_LIST_FILE}")
	elseif((NOT PARSED_ARGS_PACKAGE_NAME) AND (NOT PARSED_ARGS_PACKAGE_TARGETS))
		message(FATAL_ERROR "target_link_deploy or target_include not supplied link package name or package target by ${CMAKE_CURRENT_LIST_FILE}")
	endif()

	cmake_policy(SET CMP0079 NEW) # Allows linking to targets built externally
	set(bin_target ${PARSED_ARGS_LINK_TARGET})
	
	if(NOT(TARGET ${bin_target}))
		message(FATAL_ERROR "Could not find binary target ${bin_target}. Available targets are: ${BUILDSYSTEM_LINK_TARGETS}")
	endif()
	
	unset(lib_target)
	set(lib_target)
	if(PARSED_ARGS_PACKAGE_TARGETS)
		set(lib_target ${PARSED_ARGS_PACKAGE_TARGETS})
	endif()
	
	message("targets: ${lib_target}!")
	
	if(PARSED_ARGS_PACKAGE_NAME)
		if(NOT ${PARSED_ARGS_PACKAGE_NAME}_TARGETS)
			message(FATAL_ERROR "${PARSED_ARGS_PACKAGE_NAME}_TARGETS is not defined in the current scope. Did you forget to include target_add_*.cmake in the current CMakeLists.txt file (${CMAKE_CURRENT_LIST_FILE})?")
		else()		
			list(APPEND lib_target ${${PARSED_ARGS_PACKAGE_NAME}_TARGETS})
			#message("Should have appended targets ${${PARSED_ARGS_PACKAGE_NAME}_TARGETS}")
		endif()
	endif()
	
	#message("targets2: ${lib_target}!")
	
	foreach(REQUIRED_TARGET ${lib_target})
		if(NOT(TARGET ${REQUIRED_TARGET}))
			if(PARSED_ARGS_PACKAGE_NAME)
				message(FATAL_ERROR "Could not find target ${REQUIRED_TARGET} for library ${PARSED_ARGS_PACKAGE_NAME}. Available targets are: ${BUILDSYSTEM_LINK_TARGETS}")			
			else()
				message(FATAL_ERROR "Could not find library target ${REQUIRED_TARGET}. Available targets are: ${BUILDSYSTEM_TARGETS}")
			endif()
		endif()
	endforeach()
	
	if((NOT lib_target) OR (NOT bin_target))
		message(FATAL_ERROR "No targets associated with package ${PARSED_ARGS_PACKAGE_NAME}")
	endif()
endmacro()

# Includes target lib with specified target (binary or otherwise)
function(target_include_lib)
    cmake_parse_arguments(
        PARSED_ARGS # prefix of output variables
        "DEBUG_MESSAGES" # list of names of the boolean arguments (only defined ones will be true)
        "LINK_TARGET;PACKAGE_NAME" # list of names of mono-valued arguments
        "PACKAGE_TARGETS" # list of names of multi-valued arguments (output variables are lists)
        ${ARGN} # arguments of the function to parse, here we take the all original ones
    )

	check_include_link_args()
	if(${PARSED_ARGS_PACKAGE_NAME}_INCLUDE_DIRS)
		if(NOT(${PARSED_ARGS_PACKAGE_NAME}_NO_INCLUDE_DIRS))
			if(PARSED_ARGS_DEBUG_MESSAGES)
				message(STATUS "Adding include dirs to target ${bin_target}: ${${PARSED_ARGS_PACKAGE_NAME}_INCLUDE_DIRS}")
			endif()
			target_include_directories(${bin_target} PUBLIC ${${PARSED_ARGS_PACKAGE_NAME}_INCLUDE_DIRS})
		endif()
	else()
		if(NOT(${PARSED_ARGS_PACKAGE_NAME}_NO_INCLUDE_DIRS))
			message(WARNING "Missing ${PARSED_ARGS_PACKAGE_NAME}_INCLUDE_DIRS")
		endif()
	endif()
	foreach(REQUIRED_TARGET ${lib_target})
		if(NOT(TARGET ${REQUIRED_TARGET}))
			message(FATAL_ERROR "${REQUIRED_TARGET} target does not exist?")
		elseif(NOT(TARGET ${bin_target}))
			message(FATAL_ERROR "${bin_target} target does not exist?")
		endif()
		
		# Note: does not work for all targets (e.g. sdl2-mixer/image)
		# manual include directory kludge should work around it okay
		# if you need to fix it, you'll have to figure out what 
		# missing target property can cause 
		# "CMake Generate step failed"
		# "dependency target "X" of target "Y" does not exist.
		# and work around it.
		# I suspect the cause is trying to add dependencies to non-top-level targets
		# but I don't know how to detect whether a target is top-level or not
		
		#add_dependencies(${bin_target} ${REQUIRED_TARGET})
	endforeach()
endfunction()

# Links and deploys target lib to specified binary target
function(target_link_deploy_lib)
    cmake_parse_arguments(
        PARSED_ARGS # prefix of output variables
        "" # list of names of the boolean arguments (only defined ones will be true)
        "LINK_TARGET;PACKAGE_NAME" # list of names of mono-valued arguments
        "PACKAGE_TARGETS" # list of names of multi-valued arguments (output variables are lists)
        ${ARGN} # arguments of the function to parse, here we take the all original ones
    )
	
	#message(STATUS "PARSED_ARGS_PACKAGE_NAME: ${PARSED_ARGS_PACKAGE_NAME}")
	check_include_link_args()
	
	# if(PARSED_ARGS_PACKAGE_NAME)
		# target_include_lib(LINK_TARGET ${PARSED_ARGS_LINK_TARGET} PACKAGE_NAME ${PARSED_ARGS_PACKAGE_NAME})
	# elseif(PARSED_ARGS_PACKAGE_TARGETS)
		# target_include_lib(LINK_TARGET ${PARSED_ARGS_LINK_TARGET} PACKAGE_TARGETS ${PARSED_ARGS_PACKAGE_TARGETS})
	# else()
		# message(FATAL_ERROR "check_include_link_args failed to detect incorrrect link arguments!")
	# endif()
	
	message(STATUS "${bin_target}: Linking and deploying ${lib_target}")
	
	# Linking
	target_link_libraries(${bin_target} PUBLIC ${lib_target})
	
	# Library target deployment during build
	foreach(REQUIRED_TARGET ${lib_target})
	
		get_target_property(target_type ${REQUIRED_TARGET} TYPE)
		
		if (NOT(${target_type} STREQUAL "INTERFACE_LIBRARY"))
		
			#message(STATUS "Required link target: ${REQUIRED_TARGET}")
			add_custom_command(TARGET ${bin_target} POST_BUILD
				COMMAND ${CMAKE_COMMAND} -E copy \"$<TARGET_FILE:${REQUIRED_TARGET}>\" \"$<TARGET_FILE_DIR:${bin_target}>\"
			)
			
			# Library target deployment during installation
			install(CODE "
				execute_process(
					COMMAND \"${CMAKE_COMMAND}\" -E copy \"$<TARGET_FILE:${REQUIRED_TARGET}>\" \"${CMAKE_INSTALL_PREFIX}/bin\"
					OUTPUT_FILE \"${CMAKE_INSTALL_PREFIX}/bin/cmake_install_output.log\"
					ERROR_FILE \"${CMAKE_INSTALL_PREFIX}/bin/cmake_install_error.log\"
				)
			")
		else()
			message(STATUS "${REQUIRED_TARGET} is an INTERFACE_LIBRARY, skipping installation")
		endif()
	endforeach()
endfunction()
