cmake_minimum_required(VERSION 3.15)

if(NOT TARGET UtilogenyCore)

# Policies must be defined before project
# Defaults must be set as well to be inherited by subdirectories
cmake_policy(SET CMP0077 NEW)
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
cmake_policy(SET CMP0069 NEW)
set(CMAKE_POLICY_DEFAULT_CMP0069 NEW)
set(OVERRIDE_CXX_STANDARD "20" CACHE STRING "C++ standard")
message(STATUS "Set CXX_STANDARD to: ${OVERRIDE_CXX_STANDARD}")

set(UTILOGENY_DIR ${CMAKE_CURRENT_SOURCE_DIR})

message(STATUS "Configuring Utilogeny!")
message(STATUS "Utilogeny directory: ${UTILOGENY_DIR}")

list(APPEND CMAKE_MODULE_PATH ${UTILOGENY_DIR}/cmake)

project(Utilogeny VERSION 1.0
                  DESCRIPTION "Support library for RNA-see, Sirelphy, and Tenlaruen"
                  LANGUAGES CXX)
				  
# Remove default RTTI
string(REPLACE "/GR" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})				  

include(${UTILOGENY_DIR}/cmake/target_add_tbb.cmake)
include(${UTILOGENY_DIR}/cmake/target_config_compiler.cmake)

include_directories("..") # All include paths will be relative to parent directory

add_subdirectory(source/core)
add_subdirectory(source/qt)
add_subdirectory(source/test)	
		
message(STATUS "Utilogeny installation dir is ${CMAKE_INSTALL_PREFIX}")
		
install(TARGETS UtilogenyCore Qtilogeny DESTINATION lib)
install(TARGETS UtilogenyTest DESTINATION bin)

endif()