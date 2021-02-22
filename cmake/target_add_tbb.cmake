# Copyright 2021 Eric Fedosejevs
#

include(${UTILOGENY_DIR}/cmake/find_install_package.cmake)
include(${UTILOGENY_DIR}/cmake/target_deploy_lib.cmake)

find_install_package(FIND_PACKAGE_NAME "TBB" POSSIBLE_TARGETS "TBB;TBB::tbb")

function(target_include_tbb target)
	target_include_lib(${target} ${TBB_TARGET})
endfunction()

function(target_link_tbb target)
	target_link_deploy_lib(${target} ${TBB_TARGET})
endfunction()