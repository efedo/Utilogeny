if(NOT GTEST_FOUND)
	enable_testing()
	set(gtest_force_shared_crt ON "Use shared (DLL) run-time lib even when Google Test is built as static lib.")
	set(BUILD_GMOCK FALSE)
	set(INSTALL_GTEST FALSE)
	add_subdirectory(${UTILOGENY_DIR}/lib/gtest ${CMAKE_BINARY_DIR}/lib/gtest)
	set(GTEST_FOUND TRUE)
endif()

function(target_include_gtest target)
	target_include_directories(${target} PRIVATE ${UTILOGENY_DIR}/lib/googletest-master/googletest/include)
endfunction()

function(target_link_gtest target)
	target_include_gtest(${target})
	if (TARGET gtest)
		message(STATUS "Google test found. Linking ${target} to Google test.")
		target_link_libraries(${target} gtest gtest_main)
	else()
		message(FATAL_ERROR "Do not have Google test.")
	endif()
endfunction()