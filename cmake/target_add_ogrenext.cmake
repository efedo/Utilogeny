# Copyright 2021 Eric Fedosejevs
#

include(${UTILOGENY_DIR}/cmake/find_install_package.cmake)
include(${UTILOGENY_DIR}/cmake/target_deploy_lib.cmake)

# These need to be macros unfortunately because Qt sets various global variables required
# by automoc/rcc/ui
macro(target_include_magnum)

    cmake_parse_arguments(
        PARSED_ARGS # prefix of output variables
        "" # list of names of the boolean arguments (only defined ones will be true)
        "TARGET" # list of names of mono-valued arguments
        "CORRADE_COMPONENTS;MAGNUM_COMPONENTS" # list of names of multi-valued arguments (output variables are lists)
        ${ARGN} # arguments of the function to parse, here we take the all original ones
    )
	
	if((NOT PARSED_ARGS_TARGET) OR (NOT PARSED_ARGS_CORRADE_COMPONENTS) OR (NOT PARSED_ARGS_MAGNUM_COMPONENTS))
		message(FATAL_ERROR "target_include_magnum() or target_link_magnum() not supplied valid arguments")
	endif()

	unset(CORRADE_REQUIRED_TARGETS)
	foreach(REQUIRED_COMPONENT ${PARSED_ARGS_CORRADE_COMPONENTS})
		list(APPEND CORRADE_REQUIRED_TARGETS "Corrade::${REQUIRED_COMPONENT}")
	endforeach()
	
	unset(MAGNUM_REQUIRED_TARGETS)
	foreach(REQUIRED_COMPONENT ${PARSED_ARGS_MAGNUM_COMPONENTS})
		list(APPEND MAGNUM_REQUIRED_TARGETS "Magnum::${REQUIRED_COMPONENT}")
	endforeach()
	
	find_install_package(PACKAGE_NAME "Corrade" VCPKG_NAME "corrade" COMPONENTS "${PARSED_ARGS_CORRADE_COMPONENTS}" REQUIRED_TARGETS "${CORRADE_REQUIRED_TARGETS}")
	find_install_package(PACKAGE_NAME "Magnum" VCPKG_NAME "magnum" COMPONENTS "${PARSED_ARGS_MAGNUM_COMPONENTS}" REQUIRED_TARGETS "${MAGNUM_REQUIRED_TARGETS}")

	target_include_lib(LINK_TARGET ${PARSED_ARGS_TARGET} PACKAGE_NAME "Corrade")
	target_include_lib(LINK_TARGET ${PARSED_ARGS_TARGET} PACKAGE_NAME "Magnum")
endmacro()

macro(target_link_magnum)

    cmake_parse_arguments(
        PARSED_ARGS # prefix of output variables
        "" # list of names of the boolean arguments (only defined ones will be true)
        "TARGET" # list of names of mono-valued arguments
        "CORRADE_COMPONENTS;MAGNUM_COMPONENTS" # list of names of multi-valued arguments (output variables are lists)
        ${ARGN} # arguments of the function to parse, here we take the all original ones
    )
	
	target_include_magnum(TARGET ${PARSED_ARGS_TARGET} CORRADE_COMPONENTS ${PARSED_ARGS_CORRADE_COMPONENTS} MAGNUM_COMPONENTS ${PARSED_ARGS_MAGNUM_COMPONENTS})	
	target_link_deploy_lib(LINK_TARGET ${PARSED_ARGS_TARGET} PACKAGE_NAME "Corrade")
	target_link_deploy_lib(LINK_TARGET ${PARSED_ARGS_TARGET} PACKAGE_NAME "Magnum")
endmacro()