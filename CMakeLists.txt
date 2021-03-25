# Copyright 2021 Eric Fedosejevs
#

cmake_minimum_required(VERSION 3.15)

# These are temporary and will be replaced with find_package eventually
set(UTILOGENY_DIR "${CMAKE_CURRENT_SOURCE_DIR}" CACHE STRING "Path to Utilogeny")
include(${UTILOGENY_DIR}/cmake/setup_project.cmake)

pre_project_boilerplate(NAME "Utilogeny")

project(Utilogeny VERSION 0.2
				  DESCRIPTION "Support library for RNA-see, Sirelphy, and Tenlaruen"
				  LANGUAGES CXX)
				  
post_project_boilerplate(NAME "Utilogeny")

add_subdirectory(source/core)
add_subdirectory(source/qt)
add_subdirectory(source/test)
		
install(TARGETS UtilogenyCore Qtilogeny DESTINATION lib)
install(TARGETS UtilogenyTest DESTINATION bin)