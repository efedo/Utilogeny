option(FORCE_TBB_COMPILE "Forces compilation of TBB from source" ON)

if(NOT TBB_FOUND)

	set(TBB_USR_ROOT "${UTILOGENY_DIR}/lib/oneTBB-2021.1.1")

	# if(NOT FORCE_TBB_COMPILE)
		# message(WARNING "Warning, TBB dynamic libraries are very fussy about compiler settings of linked binary. YMMV.")

		# if(OS_REALLY_WINDOWS)
			# set(TBB_BIN_ROOT "${TBB_USR_ROOT}/bin/windows")
		# elseif(OS_REALLY_APPLE)
			# set(TBB_BIN_ROOT "${TBB_USR_ROOT}/bin/macos")
		# elseif(OS_REALLY_LINUX)
			# set(TBB_BIN_ROOT "${TBB_USR_ROOT}/bin/linux")
		# endif()

		# if(TBB_BIN_ROOT)
			# set(TBB_DIR "${TBB_BIN_ROOT}/lib/cmake/tbb")
			# find_package(TBB CONFIG)
		# endif()
	# endif()
	
	# if(TBB_FOUND)
		# message(STATUS "Found packaged TBB library (version ${TBB_VERSION}) at: ${TBB_BIN_ROOT}")
		# set(TBB_INCLUDE_DIR ${TBB_BIN_ROOT}/include)
	# else()
		message(STATUS "Will build TBB.")
		set(TBB_TEST OFF)
		add_subdirectory(${TBB_USR_ROOT}/src bin)
		set(TBB_FOUND TRUE)
		set(TBB_INCLUDE_DIR ${TBB_USR_ROOT}/src/include)
	# endif()
endif()

function(target_include_tbb target)
	target_include_directories(${target} PRIVATE ${TBB_INCLUDE_DIR})
endfunction()

function(target_link_tbb target)
	target_include_tbb(${target})
	cmake_policy(SET CMP0079 NEW) # Allows linking to targets built externally
	if (TARGET TBB)
		message(STATUS "Linking TBB to ${target}.")
		target_link_libraries(${target} TBB)
	elseif(TARGET TBB::tbb)
		message(STATUS "Linking TBB to ${target}.")
		target_link_libraries(${target} TBB::tbb)
	else()
		message(FATAL_ERROR "Do not have TBB.")
	endif()
	
	#Copy over dynamic libraries
	# if(OS_REALLY_WINDOWS)
		# if(ARCHITECTURE_64BIT)
			# set(TBB_DYNLIB_DIR "${TBB_BIN_ROOT}/redist/intel64/vc14")
		# else()
			# set(TBB_DYNLIB_DIR "${TBB_BIN_ROOT}/redist/ia32/vc14")
		# endif()
		
		#TBB file installation during install
		# install(DIRECTORY ${TBB_DYNLIB_DIR}/ DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
			# CONFIGURATIONS Debug
			# FILES_MATCHING
			# PATTERN "*debug.dll"
		# )
		
		# install(DIRECTORY ${TBB_DYNLIB_DIR}/ DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
			# CONFIGURATIONS Release
			# FILES_MATCHING
			# PATTERN "*.dll"
			# PATTERN "*debug.dll" EXCLUDE
		# )
		
	# elseif(OS_REALLY_APPLE)
		# set(TBB_DYNLIB_DIR "${TBB_BIN_ROOT}/lib")
		
		# install(DIRECTORY ${TBB_DYNLIB_DIR}/ DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
			# CONFIGURATIONS Debug
			# FILES_MATCHING
			# PATTERN "*debug*.dylib"
		# )
		# install(DIRECTORY ${TBB_DYNLIB_DIR}/ DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
			# CONFIGURATIONS Release
			# FILES_MATCHING
			# PATTERN "*.dylib"
			# PATTERN "*debug*.dylib" EXCLUDE
		# )		
	# elseif(OS_REALLY_LINUX)
		# if(ARCHITECTURE_64BIT)
			# set(TBB_DYNLIB_DIR "${TBB_BIN_ROOT}/lib/intel64/gcc4.8")
		# else()
			# set(TBB_DYNLIB_DIR "${TBB_BIN_ROOT}/lib/ia32/gcc4.8")
		# endif()

		# set(TBB_DYNLIB_DIR "${TBB_BIN_ROOT}/bin/linux")
		# install(DIRECTORY ${TBB_DYNLIB_DIR}/ DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
			# CONFIGURATIONS Debug
			# FILES_MATCHING
			# PATTERN "*debug.so*"
		# )
		# install(DIRECTORY ${TBB_DYNLIB_DIR}/ DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
			# CONFIGURATIONS Release
			# FILES_MATCHING
			# PATTERN "*.so*"
			# PATTERN "*debug.so*" EXCLUDE
		# )	
	# endif()
	
endfunction()