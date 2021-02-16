cmake_minimum_required(VERSION 3.10) # CMP0083 NEW

option(DISABLE_LTO "Disable link-time optimization (by default enabled in release mode)" OFF)

# First try default settings for GCC, Clang, MSVC
# and check if successful
# If not, manual check and set individual settings as below

include(CheckCXXCompilerFlag)

function(add_flag_if_supported flag)
	unset(COMPILER_${flag}_SUPPORTED CACHE)
	CHECK_CXX_COMPILER_FLAG("${flag}" COMPILER_${flag}_SUPPORTED)
	if (${COMPILER_${flag}_SUPPORTED})
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flag}")
		message(STATUS "Flag ${flag} ... set")
	else()
		message(STATUS "Flag ${flag} ... not supported")
	endif()
endfunction()

message(STATUS "Setting processor optimization flags")

# Native or AVX2 architecture
add_flag_if_supported("/arch:AVX") #MSVC
add_flag_if_supported("/arch:AVX2") #MSVC
add_flag_if_supported("-march=native") #GCC, Clang

add_flag_if_supported("-mfpmath=sse") # Not sure if this is still worthwhile, but just in case

# Fast floats
add_flag_if_supported("-ffast-math") #GCC, Clang
add_flag_if_supported("/fp:fast") #MSVC

# Disable run time type info
add_flag_if_supported("-fno-rtti") #GCC, Clang
add_flag_if_supported("/GR-") #MSVC

# MultiProcessorCompilation
add_flag_if_supported("/MP") #MSVC

# Multi-threaded runtime
add_flag_if_supported("/MTd") #MSVC

# Auto-parallelizer
add_flag_if_supported("/Qpar")
	
if(CMAKE_BUILD_TYPE MATCHES DEBUG)
    message(STATUS "Implementing Debug optimization profile")
	
	# Auto-parallelizer and vectorizer error messages
	add_flag_if_supported("/Qpar-report:2")
	add_flag_if_supported("/Qvec-report:2")
	
else()
    message(STATUS "Implementing Release optimization profile")
	
	# Maximize speed
	add_flag_if_supported("/O2") #MSVC equivalent to /Og /Oi /Ot /Oy /Ob2 /GF /Gy
	
	# Disable SDL and GS buffer security checks
	add_flag_if_supported("/sdl-") #MSVC
	add_flag_if_supported("/GS-") #MSVC
	
	# Whole program optimization
	add_flag_if_supported("/GL") #MSVC
endif()

if(NOT DISABLE_LTO)

	# LTO if supported:
	include(CheckIPOSupported)
	check_ipo_supported(RESULT is_ipo_supported OUTPUT lto_error)
	if(is_ipo_supported)
		set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)
		set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELWITHDEBINFO ON)
		set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_MINSIZEREL ON)
	endif()
		
endif()




