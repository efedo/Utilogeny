# Copyright 2021 Eric Fedosejevs
#

include(${UTILOGENY_DIR}/cmake/find_install_package.cmake)
include(${UTILOGENY_DIR}/cmake/target_deploy_lib.cmake)

find_install_package(PACKAGE_NAME "TBB" POSSIBLE_TARGETS "TBB;TBB::tbb")

function(target_include_tbb target)
	target_include_lib(TARGET ${target} PACKAGE_NAME "TBB")
endfunction()

function(target_link_tbb target)
	target_link_deploy_lib(TARGET ${target} PACKAGE_NAME "TBB")
endfunction()