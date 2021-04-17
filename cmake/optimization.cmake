if(NOT OPTIMIZATION_COMPLETED)

if(NOT OS_DETECTED)
	include(${UTILOGENY_DIR}/cmake/detect_platform.cmake)
endif()

option(DISABLE_LTO "Disable link-time optimization (by default enabled in release mode)" OFF)

# First try default settings for GCC, Clang, MSVC
# and check if successful
# If not, manual check and set individual settings as below

include(CheckCXXCompilerFlag)

unset(CMAKE_EXTRA_CXX_FLAGS)

function(add_flag_if_supported flag)
	unset(COMPILER_${flag}_SUPPORTED CACHE)
	CHECK_CXX_COMPILER_FLAG("${flag}" COMPILER_${flag}_SUPPORTED)
	if (${COMPILER_${flag}_SUPPORTED})
		set(CMAKE_EXTRA_CXX_FLAGS "${CMAKE_EXTRA_CXX_FLAGS} ${flag}" CACHE STRING "C++ flags" FORCE)
		message(STATUS "Flag ${flag} ... set")
	else()
		message(STATUS "Flag ${flag} ... not supported")
	endif()
endfunction()

message(STATUS "Setting processor optimization flags")

if(PLATFORM_COMPILER MATCHES "MSVC")

	# Native or AVX2 architecture
	add_flag_if_supported("/arch:AVX") #MSVC
	add_flag_if_supported("/arch:AVX2") #MSVC

	# Fast floats
	add_flag_if_supported("/fp:fast") #MSVC

	# Disable run time type info
	#add_flag_if_supported("/GR-") #MSVC

	# MultiProcessorCompilation
	add_flag_if_supported("/MP") #MSVC

	# Auto-parallelizer
	add_flag_if_supported("/Qpar")

else() # GCC, Clang, other LLVM

	# Native or AVX2 architecture
	add_flag_if_supported("-mfpmath=sse")  # Not sure if this is still worthwhile, but just in case
	add_flag_if_supported("-march=skylake") # GCC, Clang

	# Fast floats
	add_flag_if_supported("-ffast-math") #GCC, Clang

	# Disable run time type info
	#add_flag_if_supported("-fno-rtti") #GCC, Clang

endif()

message(STATUS "Selecting optimization profile based on CMake build type: ${CMAKE_BUILD_TYPE}")
	
if(CMAKE_BUILD_TYPE MATCHES "Debug")
    message(STATUS "Implementing Debug optimization profile")
	
	if(PLATFORM_COMPILER MATCHES "MSVC")
	
	# Auto-parallelizer and vectorizer error messages
	add_flag_if_supported("/Qpar-report:2")
	add_flag_if_supported("/Qvec-report:2")
	else() # GCC, Clang, other LLVM
	endif()
else()
    message(STATUS "Implementing Release optimization profile")
	
	if(PLATFORM_COMPILER MATCHES "MSVC")
	
	# Maximize speed
	add_flag_if_supported("/O2") #MSVC equivalent to /Og /Oi /Ot /Oy /Ob2 /GF /Gy
	add_flag_if_supported("/Oi")
	add_flag_if_supported("/Ot")
	add_flag_if_supported("/Oy")
	add_flag_if_supported("/Ob2")
	add_flag_if_supported("/GF")
	add_flag_if_supported("/Gy")
	
	# Disable SDL and GS buffer security checks
	add_flag_if_supported("/sdl-") #MSVC
	add_flag_if_supported("/GS-") #MSVC
	
	# Whole program optimization
	add_flag_if_supported("/GL") #MSVC
	else() # GCC, Clang, other LLVM
	endif()
endif()

if(NOT DISABLE_LTO)

	# LTO if supported:
	include(CheckIPOSupported)
	check_ipo_supported(RESULT is_ipo_supported OUTPUT lto_error)
	if(is_ipo_supported)
		set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON CACHE BOOL "Inteprocedural optimization for release mode" FORCE)
		set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELWITHDEBINFO ON CACHE BOOL "Inteprocedural optimization for relwithdebinfo mode" FORCE)
		set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_MINSIZEREL ON CACHE BOOL "Inteprocedural optimization for debug mode" FORCE)
	endif()
		
endif()

set(OPTIMIZATION_COMPLETED ON CACHE INTERNAL "Whether compiler optimization has been completed" FORCE)

else()
	message(STATUS "Skipping compiler optimization, already optimization")
endif()



