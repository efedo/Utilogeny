cmake_minimum_required(VERSION 3.10)

set(OVERRIDE_CXX_STANDARD 17)

project(Utilogeny VERSION 1.0
                  DESCRIPTION "Very nice project"
                  LANGUAGES CXX)
				  
set(UTILOGENY_DIR ${CMAKE_CURRENT_SOURCE_DIR})
message(STATUS "Utilogeny directory: ${UTILOGENY_DIR}")

include_directories("..")
include_directories("../lib/boost_1_75_0")

add_subdirectory(source/core)
add_subdirectory(source/qt)
add_subdirectory(source/test)	
			
install(TARGETS UtilogenyCore Qtilogeny DESTINATION lib)
install(TARGETS UtilogenyTest DESTINATION bin)