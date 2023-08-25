# Copyright 2021 Eric Fedosejevs
#

include(${UTILOGENY_DIR}/cmake/find_install_package.cmake)
include(${UTILOGENY_DIR}/cmake/target_deploy_lib.cmake)

function(target_include_tbb target)
	find_install_package(PACKAGE_NAME "TBB" VCPKG_NAME "TBB" REQUIRED_TARGETS "TBB::tbb")
	target_include_lib(LINK_TARGET ${target} PACKAGE_NAME "TBB")
endfunction()

function(target_link_tbb target)
	target_include_tbb(${target})
	target_link_deploy_lib(LINK_TARGET ${target} PACKAGE_NAME "TBB" PACKAGE_TARGETS "TBB::tbb")
endfunction()