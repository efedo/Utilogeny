# Copyright 2021 Eric Fedosejevs
#

include(${UTILOGENY_DIR}/cmake/git_submodule.cmake)
include(${UTILOGENY_DIR}/cmake/find_install_package.cmake)
include(${UTILOGENY_DIR}/cmake/target_deploy_lib.cmake)

function(target_include_lvgl target)
	#find_install_package(FIND_PACKAGE_NAME "Soloud" VCPKG_NAME "none" CONAN_NAME "none" GIT_REPOSITORY "https://github.com/jarikomppa/soloud.git" POSSIBLE_TARGETS "GTest::gtest;GTest::gtest_main")
	#target_include_directories(${target} PUBLIC "${UTILOGENY_DIR}/lib/soloud/include")
	#target_include_directories(${target} PUBLIC "${UTILOGENY_DIR}/lib/soloud/include")
	#target_include_directories(${target} PUBLIC "${UTILOGENY_DIR}/lib/soloud/include")
endfunction()

function(target_link_lvgl target)
	if(NOT TARGET lvgl)
		add_subdirectory("${PROJECT_SOURCE_DIR}/lib/lvgl" "${CMAKE_BINARY_DIR}/lib/lvgl")
		add_subdirectory("${PROJECT_SOURCE_DIR}/lib/lv_drivers" "${CMAKE_BINARY_DIR}/lib/lv_drivers")
		add_subdirectory("${PROJECT_SOURCE_DIR}/lib/lv_examples" "${CMAKE_BINARY_DIR}/lib/lv_examples")
		
		set_target_properties(lvgl PROPERTIES LINKER_LANGUAGE CXX)
		set_target_properties(lv_drivers PROPERTIES LINKER_LANGUAGE CXX)
		set_target_properties(lv_examples PROPERTIES LINKER_LANGUAGE CXX)
	endif()
	target_include_lvgl(${target})
	target_link_deploy_lib(LINK_TARGET ${target} PACKAGE_TARGETS "lvgl;lv_drivers;lv_examples")
endfunction()