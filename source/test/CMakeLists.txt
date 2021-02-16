cmake_minimum_required(VERSION 3.10)

include(${UTILOGENY_DIR}/cmake/target_add_googletest.cmake)

add_executable(UtilogenyTest
	pch.cpp
	pch.h
	test.cpp
)

target_precompile_headers(UtilogenyTest
	PRIVATE pch.h)

set_target_properties(UtilogenyTest PROPERTIES
            CXX_STANDARD ${OVERRIDE_CXX_STANDARD}
            CXX_EXTENSIONS OFF
            )
			
target_link_gtest(UtilogenyTest)


