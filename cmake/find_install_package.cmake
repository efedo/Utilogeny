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

macro(find_install_package)
    cmake_parse_arguments(
        PARSED_ARGS # prefix of output variables
        "INCLUDES_NOT_REQUIRED" # list of names of the boolean arguments (only defined ones will be true)
        "PACKAGE_NAME;VCPKG_NAME;CONAN_NAME;GIT_REPOSITORY" # list of names of mono-valued arguments
        "COMPONENTS;REQUIRED_TARGETS" # list of names of multi-valued arguments (output variables are lists)
        ${ARGN} # arguments of the function to parse, here we take the all original ones
    )
	# note: if it remains unparsed arguments, here, they can be found in variable PARSED_ARGS_UNPARSED_ARGUMENTS
		
	if(PARSED_ARGS_UNPARSED_ARGUMENTS)
		MESSAGE(FATAL_ERROR "${CMAKE_PROJECT_NAME}/${PROJECT_NAME}: find_install_package was called with invalid argument(s): ${PARSED_ARGS_UNPARSED_ARGUMENTS}") 
	endif()

    if(NOT PARSED_ARGS_PACKAGE_NAME)
        message(FATAL_ERROR "${CMAKE_PROJECT_NAME}/${PROJECT_NAME}: find_install_package called without a PACKAGE_NAME")
    endif()
	
	# Already have targets for package
	if(${PARSED_ARGS_PACKAGE_NAME}_TARGETS)
	
		unset(UNDEFINED_REQUIRED_TARGET)
		unset(MISSING_REQUIRED_LIST_TARGET)
		foreach(REQUIRED_TARGET ${PARSED_ARGS_REQUIRED_TARGETS})
			# Check that all required targets are defined
			if(NOT TARGET ${REQUIRED_TARGET})
				list(APPEND UNDEFINED_REQUIRED_TARGET ${REQUIRED_TARGET})
			endif()
			
			# Check that all targets are in existing target list
			unset(MISSING_LIST_TARGET)
			list(FIND ${PARSED_ARGS_PACKAGE_NAME}_TARGETS ${REQUIRED_TARGET} MISSING_LIST_TARGET)
			
			if(MISSING_LIST_TARGET)
				#message(STATUS "MISSING_LIST_TARGET: ${MISSING_LIST_TARGET}")
				list(APPEND MISSING_REQUIRED_LIST_TARGET ${REQUIRED_TARGET})
			endif()
		endforeach()
		
		if(MISSING_REQUIRED_LIST_TARGET)
			message(STATUS "${CMAKE_PROJECT_NAME}/${PROJECT_NAME}: configuring package ${PARSED_ARGS_PACKAGE_NAME}")
		elseif(UNDEFINED_REQUIRED_TARGET)
			message(STATUS "${CMAKE_PROJECT_NAME}/${PROJECT_NAME}: configuring package ${PARSED_ARGS_PACKAGE_NAME}")
			#message(STATUS "${CMAKE_PROJECT_NAME}/${PROJECT_NAME}: 		       required targets: ${PARSED_ARGS_REQUIRED_TARGETS}")
			#message(STATUS "${CMAKE_PROJECT_NAME}/${PROJECT_NAME}: 		previous listed targets: ${${PARSED_ARGS_PACKAGE_NAME}_TARGETS}")
			#message(STATUS "${CMAKE_PROJECT_NAME}/${PROJECT_NAME}: 		 missing listed targets: ${MISSING_REQUIRED_LIST_TARGET}")
			#message(STATUS "${CMAKE_PROJECT_NAME}/${PROJECT_NAME}: 			  undefined targets: ${UNDEFINED_REQUIRED_TARGET}")	
		else()
			message(STATUS "${CMAKE_PROJECT_NAME}/${PROJECT_NAME}: skipping package ${PARSED_ARGS_PACKAGE_NAME}, already adequately configured") #: ${PARSED_ARGS_REQUIRED_TARGETS}")
			return()
		endif()
	else()
		message(STATUS "${CMAKE_PROJECT_NAME}/${PROJECT_NAME}: configuring package ${PARSED_ARGS_PACKAGE_NAME}")
	endif()
	
	if(NOT PARSED_ARGS_REQUIRED_TARGETS)
		set(PARSED_ARGS_REQUIRED_TARGETS ${PARSED_ARGS_PACKAGE_NAME})
    endif()
	
	if(NOT PARSED_ARGS_VCPKG_NAME)
		set(PARSED_ARGS_VCPKG_NAME ${PARSED_ARGS_PACKAGE_NAME})
    endif()
	
	if(NOT PARSED_ARGS_CONAN_NAME)
		set(PARSED_ARGS_CONAN_NAME ${PARSED_ARGS_PACKAGE_NAME})
    endif()
	
	# Try to find existing package or install via vcpkg/conan
	if(PARSED_ARGS_COMPONENTS)
		find_package(${PARSED_ARGS_PACKAGE_NAME} COMPONENTS ${PARSED_ARGS_COMPONENTS} CONFIG) #removed required at this point, since if missing you want it to install rather than fail
	else()
		find_package(${PARSED_ARGS_PACKAGE_NAME} CONFIG) #removed required at this point, since if missing you want it to install rather than fail
	endif()
	
	# Install the package otherwise
	if(NOT ${PARSED_ARGS_PACKAGE_NAME}_FOUND)
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
				git_clone(${PARSED_ARGS_PACKAGE_NAME} ${PARSED_ARGS_GIT_REPOSITORY})		
			endif()
		else()
			message(FATAL_ERROR "${CMAKE_PROJECT_NAME}/${PROJECT_NAME}: No package manager selected or Git repository provided to retrieve package: ${PARSED_ARGS_PACKAGE_NAME}")
		endif()
		find_package(${PARSED_ARGS_PACKAGE_NAME} COMPONENTS ${PARSED_ARGS_COMPONENTS} CONFIG) #removed required since you provide a more information error message below
	endif()
	
	if(${PARSED_ARGS_PACKAGE_NAME}_FOUND)
		#message(STATUS "Found installed package: ${PARSED_ARGS_PACKAGE_NAME}")
	else()
		message(FATAL_ERROR "${CMAKE_PROJECT_NAME}/${PROJECT_NAME}: Could not find package ${PARSED_ARGS_PACKAGE_NAME} even after attempting installation.")
	endif()
	
	# Find target and collate include directories
	unset(FOUND_INCLUDE_DIR)
	set(FOUND_INCLUDE_DIR)
	
	# Check for common include directory storage variables and manually collate all possible include directories
	# (This is necessary in cases where imported targets to do correctly set target include directories,
	# which is unfortunately quite common)
	
	# For example:
	
	# Automatically tries singular and plural
	# Original and upper case
	macro(add_possible_include_dir DIR_VAR)
		if(${DIR_VAR})
			#message(STATUS "Adding include dirs from ${DIR_VAR}: ${${DIR_VAR}}")
			list(APPEND FOUND_INCLUDE_DIR ${${DIR_VAR}})
		endif()
		if(${DIR_VAR}S)
			#message(STATUS "Adding include dirs from ${DIR_VAR}S: ${${DIR_VAR}S}")
			list(APPEND FOUND_INCLUDE_DIR ${${DIR_VAR}})
		endif()
		STRING(TOUPPER "${DIR_VAR}" CAPS_DIR_VAR)
		if(${CAPS_DIR_VAR})
			#message(STATUS "Adding include dirs from ${CAPS_DIR_VAR}: ${${CAPS_DIR_VAR}}")
			list(APPEND FOUND_INCLUDE_DIR ${${CAPS_DIR_VAR}})
		endif()
		if(${CAPS_DIR_VAR}S)
			#message(STATUS "Adding include dirs from ${CAPS_DIR_VAR}S: ${${CAPS_DIR_VAR}S}")
			list(APPEND FOUND_INCLUDE_DIR ${${CAPS_DIR_VAR}})
		endif()
	endmacro()
		
	# Grab possible package-wide include directories
	# Package_INCLUDE_DIR(S) e.g. Qt5_INCLUDE_DIR(S)
	add_possible_include_dir("${PARSED_ARGS_PACKAGE_NAME}_INCLUDE_DIR")

	# Grab possible module-specific include directories
	# PackageNameModule e.g. Qt5Core_INCLUDE_DIR(S)
	# PackageName_Module e.g. Qt5_Core_INCLUDE_DIR(S)
	# PackageName::Module e.g. Qt5::Core_INCLUDE_DIR(S)
	foreach(COMPONENT ${PARSED_ARGS_COMPONENTS})
		add_possible_include_dir("${PARSED_ARGS_PACKAGE_NAME}${COMPONENT}_INCLUDE_DIR")
		add_possible_include_dir("${PARSED_ARGS_PACKAGE_NAME}_${COMPONENT}_INCLUDE_DIR")
		add_possible_include_dir("${PARSED_ARGS_PACKAGE_NAME}::${COMPONENT}_INCLUDE_DIR")
	endforeach()
	
	# Check that all required targets are present and
	# grab possible target-specific include directories
	# (necessary in cases where target name does not match module name)
	
	unset(TMP_PACKAGE_TARGETS)
	unset(FOUND_INCLUDE_DIR)
	foreach(REQUIRED_TARGET ${PARSED_ARGS_REQUIRED_TARGETS})
		if(TARGET ${REQUIRED_TARGET})
			#message(STATUS "Found target ${REQUIRED_TARGET} for package ${PARSED_ARGS_PACKAGE_NAME}")
			list(APPEND TMP_PACKAGE_TARGETS ${REQUIRED_TARGET})
			
			# Check target property include directories
			
			macro(add_target_property_include_dir property)
				get_target_property(TARGET_INCLUDE_DIRECTORIES ${REQUIRED_TARGET} ${property})
				if(TARGET_INCLUDE_DIRECTORIES)
					#message(STATUS "Adding include dirs from property ${property}: ${TARGET_INCLUDE_DIRECTORIES}")
					list(APPEND FOUND_INCLUDE_DIR ${TARGET_INCLUDE_DIRECTORIES})
				endif()
			endmacro()
			
			add_target_property_include_dir("INCLUDE_DIRECTORIES")
			add_target_property_include_dir("INTERFACE_INCLUDE_DIRECTORIES")
			add_target_property_include_dir("INTERFACE_SYSTEM_INCLUDE_DIRECTORIES")			
			
			
			# Grab possible target-specific include directories
			# Target e.g. Qt5::Core_INCLUDE_DIR(S)			
			add_possible_include_dir("${TARGET}_INCLUDE_DIR")
		
		else()
			message(FATAL_ERROR "${CMAKE_PROJECT_NAME}/${PROJECT_NAME}: Missing target ${REQUIRED_TARGET}")
		endif()
	endforeach()
	
	if(TMP_PACKAGE_TARGETS)
		set(${PARSED_ARGS_PACKAGE_NAME}_TARGETS ${TMP_PACKAGE_TARGETS} CACHE INTERNAL "" FORCE)
		message(STATUS "${CMAKE_PROJECT_NAME}/${PROJECT_NAME}: Set ${PARSED_ARGS_PACKAGE_NAME}_TARGETS to ${${PARSED_ARGS_PACKAGE_NAME}_TARGETS}")
	else()
		message(FATAL_ERROR "${CMAKE_PROJECT_NAME}/${PROJECT_NAME}: No targets found for package ${PARSED_ARGS_PACKAGE_NAME}")
	endif()
	
	if(FOUND_INCLUDE_DIR)
		unset(${PARSED_ARGS_PACKAGE_NAME}_NO_INCLUDE_DIRS)
		set(${PARSED_ARGS_PACKAGE_NAME}_INCLUDE_DIRS ${FOUND_INCLUDE_DIR} CACHE INTERNAL "" FORCE)
		#message(STATUS "${CMAKE_PROJECT_NAME}/${PROJECT_NAME}: Set ${PARSED_ARGS_PACKAGE_NAME}_INCLUDE_DIRS")
		message(STATUS "${CMAKE_PROJECT_NAME}/${PROJECT_NAME}: Set ${PARSED_ARGS_PACKAGE_NAME}_INCLUDE_DIRS to ${${PARSED_ARGS_PACKAGE_NAME}_INCLUDE_DIRS}")
	else()
		if(PARSED_ARGS_INCLUDES_NOT_REQUIRED)
			set(${PARSED_ARGS_PACKAGE_NAME}_NO_INCLUDE_DIRS "TRUE" CACHE INTERNAL "" FORCE)
		else()
			message(WARNING "${CMAKE_PROJECT_NAME}/${PROJECT_NAME}: No include directories found for package ${PARSED_ARGS_PACKAGE_NAME}")
		endif()
	endif()	
endmacro()