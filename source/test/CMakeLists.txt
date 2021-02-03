cmake_minimum_required(VERSION 3.10)

enable_testing()

#set(GTEST_ROOT "${UTILOGENY_DIR}/lib/googletest-master" CACHE PATH "Path to googletest")
#find_package(GTest REQUIRED)
#include_directories(${GTEST_INCLUDE_DIRS})

cmake_policy(SET CMP0077 NEW)
SET(gtest_force_shared_crt ON "Use shared (DLL) run-time lib even when Google Test is built as static lib.")

add_subdirectory(${UTILOGENY_DIR}/lib/googletest-master ${CMAKE_BINARY_DIR}/lib/googletest-master/out)
include_directories(${UTILOGENY_DIR}/lib/googletest-master/googletest/include)

add_executable(UtilogenyTest
	pch.cpp
	pch.h
	test.cpp
)

set_target_properties(UtilogenyTest PROPERTIES
            CXX_STANDARD ${OVERRIDE_CXX_STANDARD}
            CXX_EXTENSIONS OFF
            )

if (TARGET gtest)
	message(STATUS "Linking to Google test B.")
	target_link_libraries(UtilogenyTest gtest)
	target_link_libraries(UtilogenyTest gtest_main)
else()
	message(FATAL_ERROR "Do not have Google test.")
endif()


