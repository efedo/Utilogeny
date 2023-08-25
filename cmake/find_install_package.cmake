cmake_minimum_required(VERSION 3.11) # 3.11 for fetch_content, 3.24 for dependency providers

# No longer using submodules, too messy
# include(${UTILOGENY_DIR}/cmake/git_submodule.cmake)

# maybe update this to use FetchContent with dependency providers instead of my homemade version
# if it ever puts projects in proper namespaces and does not expose all private targets
# See https://cmake.org/cmake/help/latest/command/cmake_language.html#dependency-providers
# People seem to not like FetchContent for large projects due to namespace collisions:
# https://www.reddit.com/r/cpp/comments/wjzyfd/cmake_324_has_support_for_dependency_providers/

option(ENABLE_VCPKG "Enable vcpkg for automatic dependency retrieval" ON)
option(ENABLE_CONAN "Enable conan for automatic dependency retrieval" ON)
option(ENABLE_GIT "Enable git for automatic dependency retrieval" ON)

include(FetchContent)

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
# See here for update: https://github.com/conan-io/cmake-conan
	include(${UTILOGENY_DIR}/cmake/conan.cmake)
	
	if(NOT TARGET conan_dependencies)
		add_custom_target(conan_dependencies)
	endif()
	
	function(install_package_conan packages)
		message(FATAL_ERROR "Conan package installation not currently working!")
		foreach(package ${packages})
			message(STATUS "Will install missing dependent package \"${package}\" via conan. Please be patient") 
			conan_cmake_run(REQUIRES Catch2/2.6.0@catchorg/stable BASIC_SETUP ${package})
			add_dependencies(${package} conan_dependencies)
		endforeach()
	endfunction()
endif()

macro(find_package_custom)
	if(PARSED_ARGS_COMPONENTS)
		find_package(${PARSED_ARGS_PACKAGE_NAME} COMPONENTS ${PARSED_ARGS_COMPONENTS} HINTS ${EXTRA_PATH_HINTS} CONFIG) #removed required at this point, since if missing you want it to install rather than fail
	else()
		find_package(${PARSED_ARGS_PACKAGE_NAME} HINTS ${EXTRA_PATH_HINTS} CONFIG) #removed required at this point, since if missing you want it to install rather than fail
	endif()
endmacro()

macro(find_install_package)
    cmake_parse_arguments(
        PARSED_ARGS # prefix of output variables
        "INCLUDES_NOT_REQUIRED" # list of names of the boolean arguments (only defined ones will be true)
        "PACKAGE_NAME;VCPKG_NAME;CONAN_NAME;GIT_REPOSITORY;CMAKE_SUBDIR" # list of names of mono-valued arguments
        "COMPONENTS;REQUIRED_TARGETS;SEARCH_HINTS" # list of names of multi-valued arguments (output variables are lists)
        ${ARGN} # arguments of the function to parse, here we take the all original ones
    )
	# note: if it remains unparsed arguments, here, they can be found in variable PARSED_ARGS_UNPARSED_ARGUMENTS
		
	if(PARSED_ARGS_UNPARSED_ARGUMENTS)
		MESSAGE(FATAL_ERROR "${CMAKE_PROJECT_NAME}/${PROJECT_NAME}: find_install_package was called with invalid argument(s): ${PARSED_ARGS_UNPARSED_ARGUMENTS}") 
	elseif(NOT PARSED_ARGS_PACKAGE_NAME)
        message(FATAL_ERROR "${CMAKE_PROJECT_NAME}/${PROJECT_NAME}: find_install_package called without a PACKAGE_NAME")
	endif()

	message(STATUS "${CMAKE_PROJECT_NAME}/${PROJECT_NAME}: finding package ${PARSED_ARGS_PACKAGE_NAME}")

	# Set up some default find_package hints
	# e.g. project/lib/package, project/lib/extern/package, project/../lib/package
	unset(EXTRA_PATH_HINTS)
	set(EXTRA_PATH_HINTS)
	list(APPEND EXTRA_PATH_HINTS "${PROJECT_SOURCE_DIR}/../lib/")
	list(APPEND EXTRA_PATH_HINTS "${PROJECT_SOURCE_DIR}/../lib/${PARSED_ARGS_PACKAGE_NAME}/")
	list(APPEND EXTRA_PATH_HINTS "${PROJECT_SOURCE_DIR}/../lib/${PARSED_ARGS_PACKAGE_NAME}/cmake/")
	list(APPEND EXTRA_PATH_HINTS "${PROJECT_SOURCE_DIR}/lib/")
	list(APPEND EXTRA_PATH_HINTS "${PROJECT_SOURCE_DIR}/lib/${PARSED_ARGS_PACKAGE_NAME}/")
	list(APPEND EXTRA_PATH_HINTS "${PROJECT_SOURCE_DIR}/lib/${PARSED_ARGS_PACKAGE_NAME}/cmake/")
	list(APPEND EXTRA_PATH_HINTS "${PROJECT_SOURCE_DIR}/lib/extern/")
	list(APPEND EXTRA_PATH_HINTS "${PROJECT_SOURCE_DIR}/lib/extern/${PARSED_ARGS_PACKAGE_NAME}/")
	list(APPEND EXTRA_PATH_HINTS "${PROJECT_SOURCE_DIR}/lib/extern/${PARSED_ARGS_PACKAGE_NAME}/cmake/")

	# Add extra path hints
	if(PARSED_ARGS_SEARCH_HINTS)
		list(APPEND EXTRA_PATH_HINTS "${PARSED_ARGS_SEARCH_HINTS}")
		message(STATUS "Added hint to search path for ${PARSED_ARGS_PACKAGE_NAME}: ${PARSED_ARGS_SEARCH_HINTS}")
	endif()

	# Try to find existing package or install via vcpkg/conan
	find_package_custom()

	# Install the package otherwise
	if(NOT ${PARSED_ARGS_PACKAGE_NAME}_FOUND)

	    if(NOT PARSED_ARGS_VCPKG_NAME AND NOT PARSED_ARGS_CONAN_NAME AND NOT PARSED_ARGS_GIT_REPOSITORY)
			message(FATAL_ERROR "Not provided a valid vcpkg or conan package name or git repository for package retrieval while trying to retrieve ${PARSED_ARGS_PACKAGE_NAME}")
		endif()

		if (PARSED_ARGS_VCPKG_NAME AND (NOT PARSED_ARGS_VCPKG_NAME MATCHES "none")) # first choice vcpkg
			message(STATUS "Retrieving with first choice package manager (vcpkg)")
			install_package_vcpkg(${PARSED_ARGS_VCPKG_NAME})
			find_package_custom()
			if(NOT ${PARSED_ARGS_PACKAGE_NAME}_FOUND)
				message(WARNING "Retrieval of ${PARSED_ARGS_PACKAGE_NAME} via vcpkg failed")
			endif()
		endif()
		if(NOT ${PARSED_ARGS_PACKAGE_NAME}_FOUND AND PARSED_ARGS_CONAN_NAME AND (NOT PARSED_ARGS_CONAN_NAME MATCHES "none")) # second choice conan
			message(STATUS "Retrieving with second choice package manager (conan)")
			install_package_conan(${PARSED_ARGS_CONAN_NAME})
			find_package_custom()
			if(NOT ${PARSED_ARGS_PACKAGE_NAME}_FOUND)
				message(WARNING "Retrieval of ${PARSED_ARGS_PACKAGE_NAME} via conan failed")
			endif()
		endif()
		if(NOT ${PARSED_ARGS_PACKAGE_NAME}_FOUND AND PARSED_ARGS_GIT_REPOSITORY AND (NOT PARSED_ARGS_GIT_REPOSITORY MATCHES "none"))
			message(STATUS "No conan or vcpkg package name supplied, trying git")

			FetchContent_Declare(
				${PARSED_ARGS_PACKAGE_NAME}
				GIT_REPOSITORY ${PARSED_ARGS_GIT_REPOSITORY}
				#GIT_TAG        703bd9caab50b139428cea1aaff9974ebee5742e # release-1.10.0
				#URL      https://intranet.mycompany.com/assets/iconset_1.12.tar.gz
				#URL_HASH MD5=5588a7b18261c20068beabfb4f530b87
				#FETCHCONTENT_BASE_DIR ${CMAKE_BINARY_DIR}/lib
			)

			# Fetch content returns lower case names because CMake authors hate people
			string(TOLOWER ${PARSED_ARGS_PACKAGE_NAME} LWR_PACKAGE_NAME)

			# Fetch the content using previously declared details
			FetchContent_GetProperties(${LWR_PACKAGE_NAME})
			if(NOT ${LWR_PACKAGE_NAME}_POPULATED)
				FetchContent_Populate(${PARSED_ARGS_PACKAGE_NAME})
			endif()

			if(${LWR_PACKAGE_NAME}_POPULATED)
				message(STATUS "${PARSED_ARGS_PACKAGE_NAME} successfully populated by FetchContent")
			else()
				message(FATAL_ERROR "${PARSED_ARGS_PACKAGE_NAME} not successfully populated by FetchContent from ${PARSED_ARGS_GIT_REPOSITORY}")
			endif()

			list(APPEND EXTRA_PATH_HINTS ${${LWR_PACKAGE_NAME}_SOURCE_DIR})
			if (PARSED_ARGS_CMAKE_SUBDIR)
				list(APPEND EXTRA_PATH_HINTS "${${LWR_PACKAGE_NAME}_SOURCE_DIR}/${PARSED_ARGS_CMAKE_SUBDIR}")
				#message(STATUS "Find package paths: ${EXTRA_PATH_HINTS}" )
			endif()

			message(STATUS "Trying to find package ${PARSED_ARGS_PACKAGE_NAME} from repository with find_package()")
			find_package_custom(${EXTRA_PATH_HINTS})

			if(NOT ${PARSED_ARGS_PACKAGE_NAME}_FOUND)
				message(STATUS "find_package() failed for ${PARSED_ARGS_PACKAGE_NAME}.")

				if(NOT PARSED_ARGS_REQUIRED_TARGETS)
					message(FATAL_ERROR "Cannot manually add directory without specifying required targets")
				endif()
				
				if (NOT TARGET ${PARSED_ARGS_REQUIRED_TARGETS})
					message(STATUS "Attempting manual directory addition.")
					add_subdirectory(${${LWR_PACKAGE_NAME}_SOURCE_DIR}/${PARSED_ARGS_CMAKE_SUBDIR} ${${LWR_PACKAGE_NAME}_BINARY_DIR}/${PARSED_ARGS_CMAKE_SUBDIR} EXCLUDE_FROM_ALL)
					#set(${PARSED_ARGS_PACKAGE_NAME}_MANUALLY_FOUND "true")		
				endif()
			endif()
		endif()
	endif()
	
	# Find target and collate include directories
	unset(FOUND_INCLUDE_DIR)
	set(FOUND_INCLUDE_DIR)
	
	# Check for common include directory storage variables and manually collate all possible include directories
	# (This is necessary in cases where imported targets do not correctly set target include directories,
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
	
	unset(FOUND_INCLUDE_DIR)
	message(STATUS "Required targets to find/set: ${PARSED_ARGS_REQUIRED_TARGETS}")
	if(PARSED_ARGS_REQUIRED_TARGETS)
		foreach(REQUIRED_TARGET ${PARSED_ARGS_REQUIRED_TARGETS})
			if(TARGET ${REQUIRED_TARGET})
				#message(STATUS "Found target ${REQUIRED_TARGET} for package ${PARSED_ARGS_PACKAGE_NAME}")			
				# Check target property include directories
				macro(add_target_property_include_dir property)

					get_target_property(TARGET_INCLUDE_DIRECTORIES ${REQUIRED_TARGET} ${property})
					if(TARGET_INCLUDE_DIRECTORIES)
						#message(STATUS "Adding include dirs from property ${property}: ${TARGET_INCLUDE_DIRECTORIES}")
						list(APPEND FOUND_INCLUDE_DIR ${TARGET_INCLUDE_DIRECTORIES})
					endif()
				endmacro()
			
				# STATIC_LIBRARY, MODULE_LIBRARY, SHARED_LIBRARY, OBJECT_LIBRARY, INTERFACE_LIBRARY, EXECUTABLE
				get_target_property(type ${REQUIRED_TARGET} TYPE)

				if (NOT(${type} STREQUAL "INTERFACE_LIBRARY"))
					add_target_property_include_dir("INCLUDE_DIRECTORIES")
				endif()
				add_target_property_include_dir("INTERFACE_INCLUDE_DIRECTORIES")
				add_target_property_include_dir("INTERFACE_SYSTEM_INCLUDE_DIRECTORIES")				
			
				# Grab possible target-specific include directories
				# Target e.g. Qt5::Core_INCLUDE_DIR(S)			
				add_possible_include_dir("${TARGET}_INCLUDE_DIR")
		
			else()
				message(FATAL_ERROR "${CMAKE_PROJECT_NAME}/${PROJECT_NAME}: Missing target ${REQUIRED_TARGET}")
			endif()
		endforeach()
		message(STATUS "Setting ${PARSED_ARGS_PACKAGE_NAME}_TARGETS to ${PARSED_ARGS_REQUIRED_TARGETS}")
		set(${PARSED_ARGS_PACKAGE_NAME}_TARGETS ${PARSED_ARGS_REQUIRED_TARGETS} CACHE INTERNAL "" FORCE)
		if(PRINT_INCLUDE_TARGET_DIRS)
			message(STATUS "${CMAKE_PROJECT_NAME}/${PROJECT_NAME}: Set ${PARSED_ARGS_PACKAGE_NAME}_TARGETS to ${${PARSED_ARGS_PACKAGE_NAME}_TARGETS}")
		endif()
	else()
		message(FATAL_ERROR "${CMAKE_PROJECT_NAME}/${PROJECT_NAME}: No targets supplied for package ${PARSED_ARGS_PACKAGE_NAME}")
	endif()
	
	if(FOUND_INCLUDE_DIR)
		unset(${PARSED_ARGS_PACKAGE_NAME}_NO_INCLUDE_DIRS)
		set(${PARSED_ARGS_PACKAGE_NAME}_INCLUDE_DIRS ${FOUND_INCLUDE_DIR} CACHE INTERNAL "" FORCE)
		#message(STATUS "${CMAKE_PROJECT_NAME}/${PROJECT_NAME}: Set ${PARSED_ARGS_PACKAGE_NAME}_INCLUDE_DIRS")
		
		if(PRINT_INCLUDE_TARGET_DIRS)
			message(STATUS "${CMAKE_PROJECT_NAME}/${PROJECT_NAME}: Set ${PARSED_ARGS_PACKAGE_NAME}_INCLUDE_DIRS to ${${PARSED_ARGS_PACKAGE_NAME}_INCLUDE_DIRS}")
		endif()
	else()
		if(PARSED_ARGS_INCLUDES_NOT_REQUIRED)
			set(${PARSED_ARGS_PACKAGE_NAME}_NO_INCLUDE_DIRS "TRUE" CACHE INTERNAL "" FORCE)
		else()
			message(WARNING "${CMAKE_PROJECT_NAME}/${PROJECT_NAME}: No include directories found for package ${PARSED_ARGS_PACKAGE_NAME}")
		endif()
	endif()	
endmacro()