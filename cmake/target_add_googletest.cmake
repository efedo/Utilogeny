# Copyright 2021 Eric Fedosejevs
#

include(${UTILOGENY_DIR}/cmake/find_install_package.cmake)
include(${UTILOGENY_DIR}/cmake/target_deploy_lib.cmake)

find_install_package(FIND_PACKAGE_NAME "GTest" POSSIBLE_TARGETS "GTest::gtest;GTest::gtest_main")

function(target_include_gtest target)
	target_include_lib(${target} ${GTest_TARGET})
endfunction()

function(target_link_gtest target)
	target_link_deploy_lib(${target} ${GTest_TARGET})
	if (TARGET GTest::gtest)
		target_link_libraries(${target} GTest::gtest GTest::gtest_main) # also need to link in Google test main
	else()
		message(FATAL_ERROR "Do not have Google test.")
	endif()
endfunction()