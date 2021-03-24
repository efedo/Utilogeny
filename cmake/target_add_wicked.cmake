# Copyright 2021 Eric Fedosejevs
#

include(${UTILOGENY_DIR}/cmake/git_submodule.cmake)
include(${UTILOGENY_DIR}/cmake/find_install_package.cmake)
include(${UTILOGENY_DIR}/cmake/target_deploy_lib.cmake)
include(${UTILOGENY_DIR}/cmake/target_add_sdl.cmake)

function(target_include_wicked target)
	#find_install_package(FIND_PACKAGE_NAME "Soloud" VCPKG_NAME "none" CONAN_NAME "none" GIT_REPOSITORY "https://github.com/jarikomppa/soloud.git" POSSIBLE_TARGETS "GTest::gtest;GTest::gtest_main")
	target_include_directories(${target} PUBLIC "${PROJECT_SOURCE_DIR}/lib/WickedEngine/WickedEngine")
endfunction()

function(target_link_wicked target)
	if((NOT TARGET WickedEngine_Windows) AND (NOT TARGET WickedEngine))
		set(WICKED_TESTS OFF)
		set(WICKED_EDITOR OFF)
		set(WINDOWS_SDL ON)
		add_subdirectory("${PROJECT_SOURCE_DIR}/lib/WickedEngine" "${CMAKE_BINARY_DIR}/lib/WickedEngine")
		#target_include_sdl2(soloud)
	endif()
	target_include_wicked(${target})
	if(TARGET WickedEngine_Windows)
		set(WICKED_TARGETS "WickedEngine_Windows")
	elseif(TARGET WickedEngine)
		find_package(Threads REQUIRED) # Need threads on *nix
		set(WICKED_TARGETS "WickedEngine;Threads")
	else()
		message(FATAL_ERROR "No valid Wicked engine target detected")
	endif()
	target_link_deploy_lib(LINK_TARGET ${target} PACKAGE_TARGETS "${WICKED_TARGETS}")
endfunction()

# Copy WickedEngine shaders to build directory
macro(target_copy_wicked_shaders target)
	set(SPIRV_SHADER_SOURCE_DIR "${PROJECT_SOURCE_DIR}/lib/WickedEngine/WickedEngine/shaders/spirv/*.cso")
	set(SPIRV_SHADER_DEST_DIR "${CMAKE_CURRENT_BINARY_DIR}/../WickedEngine/shaders/spirv/")
	file(GLOB SPIRV_SHADERS "${SPIRV_SHADER_SOURCE_DIR}")
	if(NOT SPIRV_SHADERS) 
		message(WARNING "Could not gather SPIRV shaders from ${SPIRV_SHADER_SOURCE_DIR}")
	else()
		message(STATUS "Gathered SPIRV shaders from ${SPIRV_SHADER_SOURCE_DIR}")
		message(STATUS "Copying to ${SPIRV_SHADER_DEST_DIR}")
	endif()
	
	add_custom_command(TARGET "${target}" COMMAND ${CMAKE_COMMAND} -E make_directory "${SPIRV_SHADER_DEST_DIR}")
	
	foreach(SPIRV_SHADER ${SPIRV_SHADERS})
		get_filename_component(SPIRV_SHADER_NAME "${SPIRV_SHADER}" NAME)
		set(SPIRV_SHADER_OUTPUT "${SPIRV_SHADER_DEST_DIR}/${SPIRV_SHADER_NAME}")
		add_custom_command(TARGET "${target}" COMMAND ${CMAKE_COMMAND} -E copy_if_different "${SPIRV_SHADER}" "${SPIRV_SHADER_OUTPUT}")
	endforeach()

	if (WIN32)
		set(DX_SHADER_SOURCE_DIR "${PROJECT_SOURCE_DIR}/lib/WickedEngine/WickedEngine/shaders/*.cso")
		set(DX_SHADER_DEST_DIR "${CMAKE_CURRENT_BINARY_DIR}/../WickedEngine/shaders/")
		file(GLOB DX_SHADERS "${DX_SHADER_SOURCE_DIR}")
		if(NOT DX_SHADERS) 
			message(WARNING "Could not gather DX shaders from ${DX_SHADER_SOURCE_DIR}")
		else()
			message(STATUS "Gathered DX shaders from ${DX_SHADER_SOURCE_DIR}")
			message(STATUS "Copying to ${DX_SHADER_DEST_DIR}")
		endif()
		
		add_custom_command(TARGET "${target}" COMMAND ${CMAKE_COMMAND} -E make_directory "${DX_SHADER_DEST_DIR}")
		
		foreach(DX_SHADER ${DX_SHADERS})
			get_filename_component(DX_SHADER_NAME "${DX_SHADER}" NAME)
			set(DX_SHADER_OUTPUT "${DX_SHADER_DEST_DIR}/${DX_SHADER_NAME}")
			add_custom_command(TARGET "${target}" COMMAND ${CMAKE_COMMAND} -E copy_if_different "${DX_SHADER}" "${DX_SHADER_OUTPUT}")
		endforeach()	
	endif()
endmacro()