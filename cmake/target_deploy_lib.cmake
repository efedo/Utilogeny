# Copyright 2021 Eric Fedosejevs
#


# Quiet version Includes target lib with specified target (binary or otherwise)
function(target_include_lib_quiet install_target lib_target)
	if(${lib_target}_INCLUDE_DIR)
		target_include_directories(${target} PUBLIC ${${lib_target}_INCLUDE_DIR})
	endif()
	add_dependencies(${target} ${lib_target})
endfunction()


macro(check_include_link_args)
	if((NOT PARSED_ARGS_TARGET) OR ((NOT PARSED_ARGS_PACKAGE_NAME) AND (NOT PARSED_ARGS_LINK_TARGET)))
		message(FATAL_ERROR "Incorrect arguments supplied to target_link_deploy")
	else()
		if((PARSED_ARGS_PACKAGE_NAME) AND (PARSED_ARGS_LINK_TARGET))
			message(FATAL_ERROR "Should only provide one of PACKAGE_NAME and LINK_TARGET to target_link_deploy")
		endif()
		set(bin_target ${PARSED_ARGS_TARGET})
		if(NOT PARSED_ARGS_LINK_TARGET)
			if(NOT ${PARSED_ARGS_PACKAGE_NAME}_TARGET)
				message(FATAL_ERROR "${PARSED_ARGS_PACKAGE_NAME}_TARGET is not defined in the current scope. Did you forget to include target_add_*.cmake in the current CMakeLists.txt file?")
			else()
				set(lib_target ${${PARSED_ARGS_PACKAGE_NAME}_TARGET})
			endif()
		else()
			set(lib_target ${PARSED_ARGS_LINK_TARGET})
		endif()
	endif()

	cmake_policy(SET CMP0079 NEW) # Allows linking to targets built externally

	if(NOT(TARGET ${lib_target}))
		if(PARSED_ARGS_PACKAGE_NAME)
			message(FATAL_ERROR "Could not find target for library ${PARSED_ARGS_PACKAGE_NAME}. Available targets are: ${BUILDSYSTEM_TARGETS}")			
		else()
			message(FATAL_ERROR "Could not find library target ${lib_target}. Available targets are: ${BUILDSYSTEM_TARGETS}")
		endif()
	endif()

	if(NOT(TARGET ${bin_target}))
		message(FATAL_ERROR "Could not find binary target ${lib_target}. Available targets are: ${BUILDSYSTEM_TARGETS}")
	endif()
endmacro()

# Includes target lib with specified target (binary or otherwise)
function(target_include_lib)
    cmake_parse_arguments(
        PARSED_ARGS # prefix of output variables
        "" # list of names of the boolean arguments (only defined ones will be true)
        "TARGET;PACKAGE_NAME;LINK_TARGET" # list of names of mono-valued arguments
        "POSSIBLE_TARGETS" # list of names of multi-valued arguments (output variables are lists)
        ${ARGN} # arguments of the function to parse, here we take the all original ones
    )

	check_include_link_args()

	target_include_lib_quiet(${bin_target} ${lib_target})
endfunction()

# Links and deploys target lib to specified binary target
function(target_link_deploy_lib)
    cmake_parse_arguments(
        PARSED_ARGS # prefix of output variables
        "" # list of names of the boolean arguments (only defined ones will be true)
        "TARGET;PACKAGE_NAME;LINK_TARGET" # list of names of mono-valued arguments
        "POSSIBLE_TARGETS" # list of names of multi-valued arguments (output variables are lists)
        ${ARGN} # arguments of the function to parse, here we take the all original ones
    )
	
	check_include_link_args()
	
	target_include_lib_quiet(${bin_target} ${lib_target})
	message(STATUS "${bin_target}: Linking and deploying ${lib_target}")
	
	# Linking
	target_link_libraries(${bin_target} ${lib_target})
	
	# If dynamic, need to install
	get_target_property(target_type ${lib_target} TYPE)
	if (target_type STREQUAL DYNAMIC_LIBRARY)
		
		# Library target deployment during build
		add_custom_command(TARGET ${bin_target} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_if_different \"$<TARGET_FILE:${lib_target}>\" \"$<TARGET_FILE_DIR:${bin_target}>\"
			COMMENT "Deploying ${lib_target} (during build)..."
		)
			
		# Library target deployment during installation
		install(CODE "
			execute_process(
				COMMAND ${CMAKE_COMMAND} -E copy_if_different \"$<TARGET_FILE:${lib_target}>\" \"${CMAKE_INSTALL_PREFIX}/bin>\"
				COMMENT \"Deploying ${lib_target} (during install)...\"
			)
		")
	endif()

endfunction()
