# Copyright 2021-24 Eric Fedosejevs
#

cmake_minimum_required(VERSION 3.25)

add_subdirectory(lib)
if(UTILOGENY_TEST)
	add_subdirectory(test)
endif()

install(TARGETS UtilogenyCore DESTINATION lib)
if(UTILOGENY_TEST)
	install(TARGETS UtilogenyCoreTest DESTINATION bin)
endif()