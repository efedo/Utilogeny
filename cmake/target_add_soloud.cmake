# Copyright 2021 Eric Fedosejevs
#

include(${UTILOGENY_DIR}/cmake/git_submodule.cmake)
include(${UTILOGENY_DIR}/cmake/find_install_package.cmake)
include(${UTILOGENY_DIR}/cmake/target_deploy_lib.cmake)
include(${UTILOGENY_DIR}/cmake/target_add_SDL.cmake)

#find_install_package(FIND_PACKAGE_NAME "Soloud" VCPKG_NAME "none" CONAN_NAME "none" GIT_REPOSITORY "https://github.com/jarikomppa/soloud.git" POSSIBLE_TARGETS "GTest::gtest;GTest::gtest_main")
#submodule already added, soloud doens't have proper config file

function(target_include_soloud target)
	target_include_directories(${target} PUBLIC "${UTILOGENY_DIR}/lib/soloud/include")
	#target_include_lib(${target} ${GTest_TARGET})
endfunction()

function(target_link_soloud target)
	if(NOT TARGET soloud)
		set(SOLOUD_BUILD_DEMOS OFF)
		file(REMOVE "${UTILOGENY_DIR}/lib/soloud/contrib/cmake/FindSDL2.cmake") # delete obsolete SDL2 find if it exists, which screws up debug vs. release dll linking
		add_subdirectory("${UTILOGENY_DIR}/lib/soloud/contrib" "${CMAKE_BINARY_DIR}/lib")
		target_include_sdl2(soloud)
	endif()
	target_link_deploy_lib(TARGET ${target} LINK_TARGET "soloud")
endfunction()