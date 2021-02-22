# Copyright 2021 Eric Fedosejevs
#

include(${UTILOGENY_DIR}/cmake/find_install_package.cmake)
include(${UTILOGENY_DIR}/cmake/target_deploy_lib.cmake)

option(ENABLE_SDL2 "Enable SDL2" ON)
option(ENABLE_SDL2_MIXER "Enable audio support using SDL2_mixer" ON)
option(ENABLE_SDL2_IMAGE "Enable image support using SDL2_image" ON)

set(SDL2_BUILDING_LIBRARY 1) # Omit SDL main()

if(ENABLE_SDL2_MIXER AND NOT ENABLE_SDL2)
	message(FATAL_ERROR "Cannot enable SDL2 mixer without enabling SDL2!")
endif()

if(ENABLE_SDL2_IMAGE AND NOT ENABLE_SDL2)
	message(FATAL_ERROR "Cannot enable SDL2 image without enabling SDL2!")
endif()

if(ENABLE_SDL2)
	find_install_package(FIND_PACKAGE_NAME "SDL2" POSSIBLE_TARGETS "SDL2::SDL2;SDL2::SDL2main")
endif()
if(ENABLE_SDL2_MIXER)
	find_install_package(FIND_PACKAGE_NAME "sdl2-mixer" VCPKG_NAME "sdl2-mixer" POSSIBLE_TARGETS "SDL2::SDL2_mixer")
endif()
if(ENABLE_SDL2_IMAGE)
	find_install_package(FIND_PACKAGE_NAME "sdl2-image" VCPKG_NAME "sdl2-image" POSSIBLE_TARGETS "SDL2::SDL2_image")
endif()

function(target_include_sdl2 target)
	if(ENABLE_SDL2)
		target_include_lib(${target} ${SDL2_TARGET})
	endif()
	if(ENABLE_SDL2_MIXER)
		target_include_lib(${target} ${sdl2-mixer_TARGET})
	endif()
	if(ENABLE_SDL2_IMAGE)
		target_include_lib(${target} ${sdl2-image_TARGET})
	endif()
endfunction()

function(target_link_sdl2 target)
	if(ENABLE_SDL2)
		target_link_deploy_lib(${target} ${SDL2_TARGET})
	endif()
	if(ENABLE_SDL2_MIXER)
		target_link_deploy_lib(${target} ${sdl2-mixer_TARGET})
	endif()	
	if(ENABLE_SDL2_IMAGE)
		target_link_deploy_lib(${target} ${sdl2-image_TARGET})
	endif()
endfunction()