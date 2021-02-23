# Copyright 2021 Eric Fedosejevs
#

include(${UTILOGENY_DIR}/cmake/find_install_package.cmake)
include(${UTILOGENY_DIR}/cmake/target_deploy_lib.cmake)

find_install_package(PACKAGE_NAME "GTest" POSSIBLE_TARGETS "GTest::gtest")

function(target_include_gtest target)
	target_include_lib(TARGET ${target} PACKAGE_NAME "GTest")
endfunction()

function(target_link_gtest target)
	target_link_deploy_lib(TARGET ${target} PACKAGE_NAME "GTest")
	if (TARGET GTest::gtest)
		target_link_libraries(${target} GTest::gtest_main) # also need to link in Google test main
	else()
		message(FATAL_ERROR "Do not have Google test.")
	endif()
endfunction()