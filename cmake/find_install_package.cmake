include(${UTILOGENY_DIR}/cmake/git_submodule.cmake)

option(ENABLE_VCPKG "Enable vcpkg for automatic dependency retrieval" ON)
option(ENABLE_CONAN "Enable conan for automatic dependency retrieval" ON)

if(ENABLE_VCPKG)
	include(${UTILOGENY_DIR}/cmake/AutoVcpkg.cmake)
	function(install_package_vcpkg packages)
		foreach(package ${packages})
			message(STATUS "Installing missing dependent package \"${package}\" via vcpkg. Please be patient") 
			vcpkg_install(${packages})
			message(STATUS "Installation of \"${package}\" complete")
		endforeach()
	endfunction()
endif()

if(ENABLE_CONAN)
	include(${UTILOGENY_DIR}/cmake/conan.cmake)
	
	if(NOT TARGET conan_dependencies)
		add_custom_target(conan_dependencies)
	endif()
	
	function(install_package_conan packages)
		foreach(package ${packages})
			message(STATUS "Will install missing dependent package \"${package}\" via conan. Please be patient") 
			conan_cmake_run(REQUIRES Catch2/2.6.0@catchorg/stable BASIC_SETUP ${package})
			add_dependencies(conan_dependencies ${package})
		endforeach()
	endfunction()
endif()

# Prefer whichever of vcpkg and conan is already installed
# Otherwise prefer vcpkg
function(select_PREFERRED_PKG_MANAGER)
	if(PREFERRED_PKG_MANAGER)
		return()
	endif()

	if(CMAKE_TOOLCHAIN_FILE)
		get_filename_component(VCPKG_TOOLCHAIN_PATH ${CMAKE_TOOLCHAIN_FILE} NAME)
		if (VCPKG_TOOLCHAIN_PATH MATCHES "vcpkg.cmake")
			message(STATUS "Found existing vcpkg installation")
			set(ALREADY_HAVE_VCPKG)
		else()
			message(STATUS "Did not find existing vcpkg installation")
		endif()
	endif()

	find_program(CONAN conan)
	if(CONAN)
		message(STATUS "Found existing conan installation")
		set(ALREADY_HAVE_CONAN)
	else()
		message(STATUS "Did not find existing conan installation")
	endif()
	
	# vcpkg enabled and have vcpkg
	if(ENABLE_VCPKG)
		if(ALREADY_HAVE_VCPKG)
			set(PREFERRED_PKG_MANAGER "vcpkg" CACHE STRING "Selected package manager" FORCE)
		endif()
	endif()

	#conan enabled and have conan
	if(ENABLE_CONAN AND NOT PREFERRED_PKG_MANAGER)
		if(ALREADY_HAVE_CONAN)
			set(PREFERRED_PKG_MANAGER "conan" CACHE STRING "Selected package manager" FORCE)
		endif()
	endif()

	if(NOT PREFERRED_PKG_MANAGER)
		if(ENABLE_VCPKG)
			set(PREFERRED_PKG_MANAGER "vcpkg" CACHE STRING "Selected package manager" FORCE)
		elseif(ENABLE_CONAN)
			set(PREFERRED_PKG_MANAGER "conan" CACHE STRING "Selected package manager" FORCE)
		else()
			message(ERROR "No package manager enabled, you will have to hope manual git dependency installations work okay!")
		endif()
	endif()
endfunction()

function(find_install_package)
    cmake_parse_arguments(
        PARSED_ARGS # prefix of output variables
        "" # list of names of the boolean arguments (only defined ones will be true)
        "FIND_PACKAGE_NAME;VCPKG_NAME;CONAN_NAME;GIT_REPOSITORY" # list of names of mono-valued arguments
        "POSSIBLE_TARGETS" # list of names of multi-valued arguments (output variables are lists)
        ${ARGN} # arguments of the function to parse, here we take the all original ones
    )
	
	if(PARSED_ARGS_UNPARSED_ARGUMENTS)
		MESSAGE(FATAL_ERROR "find_install_package was called with invalid argument(s): ${PARSED_ARGS_UNPARSED_ARGUMENTS}") 
	endif()
	
    # note: if it remains unparsed arguments, here, they can be found in variable PARSED_ARGS_UNPARSED_ARGUMENTS
    if(NOT PARSED_ARGS_FIND_PACKAGE_NAME)
        message(FATAL_ERROR "You must provide a find_package name")
    endif()
	
	if(NOT PARSED_ARGS_POSSIBLE_TARGETS)
		set(PARSED_ARGS_POSSIBLE_TARGETS ${PARSED_ARGS_FIND_PACKAGE_NAME})
    endif()
	
	if(NOT PARSED_ARGS_VCPKG_NAME)
		set(PARSED_ARGS_VCPKG_NAME ${PARSED_ARGS_FIND_PACKAGE_NAME})
    endif()
	
	if(NOT PARSED_ARGS_CONAN_NAME)
		set(PARSED_ARGS_CONAN_NAME ${PARSED_ARGS_FIND_PACKAGE_NAME})
    endif()
	
	# # Check if you already have a suitable target
	# foreach(POSSIBLE_TARGET ${PARSED_ARGS_POSSIBLE_TARGETS})
		# if(TARGET ${POSSIBLE_TARGET})
			# message(STATUS "Found existing target: ${POSSIBLE_TARGET} for package: ${PARSED_ARGS_FIND_PACKAGE_NAME}")			
			# set(FOUND_INCLUDE_DIR ${${PARSED_ARGS_FIND_PACKAGE_NAME}_INCLUDE_DIR} ${${PARSED_ARGS_FIND_PACKAGE_NAME}_INCLUDE_DIRS})		
			# message(STATUS "Found corresponding include directory: ${FOUND_INCLUDE_DIR}")
			# set(${PARSED_ARGS_FIND_PACKAGE_NAME}_INCLUDE_DIR ${FOUND_INCLUDE_DIR} PARENT_SCOPE)
			# set(${PARSED_ARGS_FIND_PACKAGE_NAME}_TARGET ${POSSIBLE_TARGET} PARENT_SCOPE)
			# return()
		# endif()
	# endforeach()

	# Try to find existing package or install via vcpkg/conan
	find_package(${PARSED_ARGS_FIND_PACKAGE_NAME} CONFIG) #removed required at this point, since if missing you want it to install rather than fail
	if(${PARSED_ARGS_FIND_PACKAGE_NAME}_FOUND)
		message(STATUS "Found existing package: ${PARSED_ARGS_FIND_PACKAGE_NAME}")
	else()
		select_PREFERRED_PKG_MANAGER()
		unset(INSTALL_OPTION_FOUND)
		#try preferred package manager first
		if(PREFERRED_PKG_MANAGER MATCHES "vcpkg")
			if (NOT PARSED_ARGS_VCPKG_NAME MATCHES "none") # first choice vcpkg
				install_package_vcpkg(${PARSED_ARGS_VCPKG_NAME})
				set(INSTALL_OPTION_FOUND)
			elseif(NOT PARSED_ARGS_CONAN_NAME MATCHES "none") # second choice conan
				install_package_conan(${PARSED_ARGS_CONAN_NAME})
				set(INSTALL_OPTION_FOUND)
			endif()
		elseif((NOT INSTALL_OPTION_FOUND) AND (PREFERRED_PKG_MANAGER MATCHES "conan"))
			if(NOT PARSED_ARGS_CONAN_NAME MATCHES "none") # first choice conan
				install_package_conan(${PARSED_ARGS_CONAN_NAME})
				set(INSTALL_OPTION_FOUND)
			elseif(NOT PARSED_ARGS_VCPKG_NAME MATCHES "none") # second choice vcpkg
				install_package_vcpkg(${PARSED_ARGS_VCPKG_NAME})
				set(INSTALL_OPTION_FOUND)
			endif()
		elseif(NOT INSTALL_OPTION_FOUND)
			# try git installation instead
			if(PARSED_ARGS_GIT_REPOSITORY)
				git_submodule_download(${PARSED_ARGS_FIND_PACKAGE_NAME} ${PARSED_ARGS_GIT_REPOSITORY})		
			endif()
		else()
			message(FATAL_ERROR "No package manager selected or Git repository provided to retrieve package: ${PARSED_ARGS_FIND_PACKAGE_NAME}")
		endif()
		find_package(${PARSED_ARGS_FIND_PACKAGE_NAME} CONFIG) #removed required since you provide a more information error message below
	endif()
	
	if(NOT ${PARSED_ARGS_FIND_PACKAGE_NAME}_FOUND)
		message(FATAL_ERROR "Could not find package ${PARSED_ARGS_FIND_PACKAGE_NAME} even after attempting installation.")
	endif()
	
	# Find target
	foreach(POSSIBLE_TARGET ${PARSED_ARGS_POSSIBLE_TARGETS})
		if(TARGET ${POSSIBLE_TARGET})
			message(STATUS "Found target: ${POSSIBLE_TARGET} for package: ${PARSED_ARGS_FIND_PACKAGE_NAME}")
			get_target_property(TARGET_INCLUDE_DIRECTORIES ${POSSIBLE_TARGET} INCLUDE_DIRECTORIES)
			get_target_property(TARGET_INTERFACE_INCLUDE_DIRECTORIES ${POSSIBLE_TARGET} INTERFACE_INCLUDE_DIRECTORIES)
			get_target_property(TARGET_INTERFACE_SYSTEM_INCLUDE_DIRECTORIES ${POSSIBLE_TARGET} INTERFACE_SYSTEM_INCLUDE_DIRECTORIES)
			
			if(TARGET_INCLUDE_DIRECTORIES)
				list(APPEND FOUND_INCLUDE_DIR ${TARGET_INCLUDE_DIRECTORIES})
			endif()
			
			if(TARGET_INTERFACE_INCLUDE_DIRECTORIES)
				list(APPEND FOUND_INCLUDE_DIR ${TARGET_INTERFACE_INCLUDE_DIRECTORIES})
			endif()
			
			if(TARGET_INTERFACE_SYSTEM_INCLUDE_DIRECTORIES)
				list(APPEND FOUND_INCLUDE_DIR ${TARGET_INTERFACE_SYSTEM_INCLUDE_DIRECTORIES})
			endif()

			message(STATUS "Found possible corresponding include directories: ${FOUND_INCLUDE_DIR}")		
			set(${PARSED_ARGS_FIND_PACKAGE_NAME}_INCLUDE_DIR ${FOUND_INCLUDE_DIR} PARENT_SCOPE)
			set(${PARSED_ARGS_FIND_PACKAGE_NAME}_TARGET ${POSSIBLE_TARGET} PARENT_SCOPE)
			return()
		endif()
	endforeach()
	
	message(FATAL_ERROR "Do not have valid target for found package ${PARSED_ARGS_FIND_PACKAGE_NAME} (checked possible targets: ${PARSED_ARGS_POSSIBLE_TARGETS}).")
endfunction()
