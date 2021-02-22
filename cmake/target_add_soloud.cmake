# Copyright 2021 Eric Fedosejevs
#

include(${UTILOGENY_DIR}/cmake/find_install_package.cmake)
include(${UTILOGENY_DIR}/cmake/target_deploy_lib.cmake)

#find_install_package(FIND_PACKAGE_NAME "Soloud" VCPKG_NAME "none" CONAN_NAME "none" GIT_REPOSITORY "https://github.com/jarikomppa/soloud.git" POSSIBLE_TARGETS "GTest::gtest;GTest::gtest_main")
#submodule already added, soloud doens't have proper config file

function(target_include_soloud target)
	target_include_directories(${target} PUBLIC "${UTILOGENY_DIR}/lib/extern/soloud/include")
	#target_include_lib(${target} ${GTest_TARGET})
endfunction()

function(target_link_soloud target)
	if(NOT TARGET soloud)
		add_subdirectory("${UTILOGENY_DIR}/lib/extern/soloud/contrib" "${CMAKE_BINARY_DIR}/lib")
	endif()
	target_link_deploy_lib(${target} soloud)
endfunction()