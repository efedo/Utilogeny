# Copyright 2021-24 Eric Fedosejevs
#

cmake_minimum_required(VERSION 3.24)

add_subdirectory(lib)
add_subdirectory(examples)
if(UTILOGENY_TEST)
	add_subdirectory(test)
endif()

install(TARGETS UtilogenyGui DESTINATION lib)
install(TARGETS UtilogenyGuiExample1 DESTINATION bin)
if(UTILOGENY_TEST)
	install(TARGETS UtilogenyGuiTest DESTINATION bin)
endif()