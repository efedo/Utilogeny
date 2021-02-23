if(NOT OS_DETECTED)
	include(${UTILOGENY_DIR}/cmake/detect_platform.cmake)
endif()

if(NOT OPTIMIZATION_COMPLETED)
	include(${UTILOGENY_DIR}/cmake/optimization.cmake)
endif()

function(target_config_compiler target)
	set_target_properties(${target} PROPERTIES COMPILE_FLAGS ${CMAKE_EXTRA_CXX_FLAGS})
	if(CMAKE_OS)
		list(APPEND ${target}AdditionalDefinitions "OS_${CMAKE_OS}")
	else()
		message(FATAL_ERROR "OS not successfully detected.")
	endif()

	if(PLATFORM_64BIT)
		list(APPEND ${target}AdditionalDefinitions "PLATFORM_64BIT")
	else()
		message(FATAL_ERROR "Memory architecture not detected.")
	endif()
	
	if(PLATFORM_COMPILER)	
		string(TOUPPER ${PLATFORM_COMPILER} PLATFORM_COMPILER_UPPER)
		string(TOUPPER ${CMAKE_CXX_COMPILER_ID} CMAKE_CXX_COMPILER_ID_UPPER)
		
		list(APPEND ${target}AdditionalDefinitions "COMPILER_LINEAGE=${PLATFORM_COMPILER_UPPER}")
		list(APPEND ${target}AdditionalDefinitions "COMPILER_LINEAGE_${PLATFORM_COMPILER_UPPER}")
		list(APPEND ${target}AdditionalDefinitions "COMPILER=${CMAKE_CXX_COMPILER_ID_UPPER}")
		list(APPEND ${target}AdditionalDefinitions "COMPILER_${CMAKE_CXX_COMPILER_ID_UPPER}")
		list(APPEND ${target}AdditionalDefinitions "COMPILER_VERSION=${CMAKE_CXX_COMPILER_VERSION}")
#		list(APPEND ${target}AdditionalDefinitions "COMPILER_VERSION_${CMAKE_CXX_COMPILER_VERSION}")
		
		if(PLATFORM_COMPILER MATCHES "CLANG") # Clang also uses LLVM backend
			add_compile_definitions(COMPILER_LINEAGE_LLVM)
			message(STATUS "Added COMPILER_LLVM compiler definition")
		endif()	
	else()
		message(FATAL_ERROR "Compiler not detected.")
	endif()
	
	message(STATUS "${target}: Adding compiler definitions: ${${target}AdditionalDefinitions}")
	target_compile_definitions(${target} PUBLIC ${${target}AdditionalDefinitions})
	
endfunction()

