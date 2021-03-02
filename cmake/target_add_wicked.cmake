# Copyright 2021 Eric Fedosejevs
#

include(${UTILOGENY_DIR}/cmake/git_submodule.cmake)
include(${UTILOGENY_DIR}/cmake/find_install_package.cmake)
include(${UTILOGENY_DIR}/cmake/target_deploy_lib.cmake)
include(${UTILOGENY_DIR}/cmake/target_add_sdl.cmake)

function(target_include_wicked target)
	#find_install_package(FIND_PACKAGE_NAME "Soloud" VCPKG_NAME "none" CONAN_NAME "none" GIT_REPOSITORY "https://github.com/jarikomppa/soloud.git" POSSIBLE_TARGETS "GTest::gtest;GTest::gtest_main")
	#target_include_directories(${target} PUBLIC "${UTILOGENY_DIR}/lib/wicked/include")
endfunction()

function(target_link_soloud target)
	if((NOT TARGET WickedEngine_Windows) AND (NOT TARGET WickedEngine))
		add_subdirectory("${PROJECT_SOURCE_DIR}/lib/WickedEngine" "${CMAKE_BINARY_DIR}/lib/WickedEngine")
		#target_include_sdl2(soloud)
	endif()
	target_include_wicked(${target})
	if(TARGET WickedEngine_Windows)
		set(WICKED_TARGET WickedEngine_Windows)
	elseif(TARGET WickedEngine)
		set(WICKED_TARGET WickedEngine)
	else()
		message(FATAL_ERROR "No valid Wicked engine target detected")
	endif()
	target_link_deploy_lib(LINK_TARGET ${target} PACKAGE_TARGETS "${WICKED_TARGET}")
endfunction()