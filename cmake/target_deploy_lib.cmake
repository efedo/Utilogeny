# Copyright 2021 Eric Fedosejevs
#

# Includes target lib with specified target (binary or otherwise)
function(target_include_lib install_target lib_target)
	message(STATUS "Adding ${lib_target} dependency and include dir to target ${target}: ${${lib_target}_INCLUDE_DIR}")
	target_include_directories(${target} PUBLIC ${${lib_target}_INCLUDE_DIR})
	add_dependencies(${target} ${lib_target})
endfunction()

# Links and deploys target lib to specified binary target
function(target_link_deploy_lib bin_target lib_target)
	cmake_policy(SET CMP0079 NEW) # Allows linking to targets built externally
	target_include_lib(${bin_target} ${lib_target})
	message(STATUS "Linking and deploying library ${lib_target} with binary target ${bin_target}.")

	if(NOT(TARGET ${lib_target}))
		message(FATAL_ERROR "Could not find library target for deployment: ${lib_target}")
	endif()

	if(NOT(TARGET ${bin_target}))
		message(FATAL_ERROR "Could not find binary target: ${bin_target}")
	endif()
	
	# Linking
	target_link_libraries(${bin_target} ${lib_target})
	
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

endfunction()
