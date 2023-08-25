# Copyright 2021 Eric Fedosejevs
#

include(${UTILOGENY_DIR}/cmake/find_install_package.cmake)
include(${UTILOGENY_DIR}/cmake/target_deploy_lib.cmake)

function(target_include_gtest target)
	find_install_package(PACKAGE_NAME "GTest" VCPKG_NAME "gtest" REQUIRED_TARGETS "GTest::gtest;GTest::gtest_main")
	target_include_lib(LINK_TARGET ${target} PACKAGE_NAME "GTest")
endfunction()

function(target_link_gtest target)
	target_include_gtest(${target})
	target_link_deploy_lib(LINK_TARGET ${target} PACKAGE_NAME "GTest")
endfunction()