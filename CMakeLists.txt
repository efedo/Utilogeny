cmake_minimum_required(VERSION 3.10)

set(OVERRIDE_CXX_STANDARD 17)

project(Utilogeny VERSION 1.0
                  DESCRIPTION "Support library for RNA-see, Sirelphy, and Tenlaruen"
                  LANGUAGES CXX)
				  
set(UTILOGENY_DIR ${CMAKE_CURRENT_SOURCE_DIR})

message(STATUS "Utilogeny directory: ${UTILOGENY_DIR}")

include(${UTILOGENY_DIR}/cmake/detect_platform.cmake)
include(${UTILOGENY_DIR}/cmake/optimization.cmake)
include(${UTILOGENY_DIR}/cmake/target_add_googletest.cmake)
include(${UTILOGENY_DIR}/cmake/target_add_tbb.cmake)

include_directories("..")
include_directories("${UTILOGENY_DIR}/lib") # for Boost

add_subdirectory(source/core)
add_subdirectory(source/qt)
add_subdirectory(source/test)	
			
install(TARGETS UtilogenyCore Qtilogeny DESTINATION lib)
install(TARGETS UtilogenyTest DESTINATION bin)