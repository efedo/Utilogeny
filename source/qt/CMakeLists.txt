cmake_minimum_required(VERSION 3.10)

add_library(Qtilogeny STATIC
	precomp.cpp
	precomp.h
)

target_precompile_headers(Qtilogeny
	PRIVATE precomp.h)

set_target_properties(Qtilogeny PROPERTIES
            CXX_STANDARD ${OVERRIDE_CXX_STANDARD}
            CXX_EXTENSIONS OFF
            )	