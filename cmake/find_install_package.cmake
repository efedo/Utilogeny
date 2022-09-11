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

if(ENABLE_GIT)
# Clean this up based on https://github.com/adobe/lagrange/tree/72f9a5447b6803245d43a37a18b76e59c16fbda8/cmake/recipes/external
	macro(install_package_git)
		message(STATUS "No vcpkg or conan package provided, attempting retrieval of git archive: ${PARSED_ARGS_GIT_REPOSITORY}")
		if(PARSED_ARGS_GIT_REPOSITORY AND (NOT PARSED_ARGS_GIT_REPOSITORY MATCHES "none"))
		
			#git_clone("${PARSED_ARGS_PACKAGE_NAME}" "${repo}")

			#if (PARSED_ARGS_BUILD_GIT)
				# Include build step (for projects that need to be pre-built)		
				set(build_file "${CMAKE_BINARY_DIR}/cmake/${PARSED_ARGS_PACKAGE_NAME}_build.cmake")
				configure_file("build_external.cmake" "${build_file}")
									
				message(STATUS "Installing ${PARSED_ARGS_PACKAGE_NAME} via Git")
									
				# Configure external project
				execute_process(
					COMMAND ${CMAKE_COMMAND} ${build_file}
					WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/lib/${PARSED_ARGS_PACKAGE_NAME}
				)

				# Build external project
				execute_process(
					COMMAND ${CMAKE_COMMAND} --build ${build_file}
				)
			# else()
				# # Exclude build (just download)
				# include(FetchContent)
				# FetchContent_Populate(
					# ${PARSED_ARGS_PACKAGE_NAME}                             # Recommendation: Stick close to the original name.
					# GIT_REPOSITORY 	${PARSED_ARGS_GIT_REPOSITORY}
					# #SOURCE_DIR     xxxlib              						# (Relative) path within in the build directory.
				# )
		
			# endif()

		endif()
	endmacro()
endif()

# Prefer whichever of vcpkg and conan is already installed
# Otherwise prefer vcpkg
function(select_PREFERRED_PKG_MANAGER)

	message(STATUS "Selecting preferred package manager")

	if(PREFERRED_PKG_MANAGER)
		message(STATUS "Preferred package manager is: ${PREFERRED_PKG_MANAGER} ")
		return()
	endif()

	message(STATUS "CMAKE toolchain file: ${CMAKE_TOOLCHAIN_FILE} ")

	if(CMAKE_TOOLCHAIN_FILE)
		get_filename_component(VCPKG_TOOLCHAIN_PATH ${CMAKE_TOOLCHAIN_FILE} NAME)
		if (VCPKG_TOOLCHAIN_PATH MATCHES "vcpkg.cmake")
			message(STATUS "Found existing vcpkg installation: ${CMAKE_TOOLCHAIN_FILE}")
			set(ALREADY_HAVE_VCPKG)
		else()
			message(STATUS "Did not find existing vcpkg installation")
		endif()
	else()
		message(STATUS "Did not find vcpkg toolchain file")
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

	# conan enabled and have conan
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
        "COMPONENTS;REQUIRED_TARGETS;SEARCH_HINTS" # list of names of multi-valued arguments (output variables are lists)
        ${ARGN} # arguments of the function to parse, here we take the all original ones
    )
	# note: if it remains unparsed arguments, here, they can be found in variable PARSED_ARGS_UNPARSED_ARGUMENTS
		
	if(PARSED_ARGS_UNPARSED_ARGUMENTS)
		MESSAGE(FATAL_ERROR "${CMAKE_PROJECT_NAME}/${PROJECT_NAME}: find_install_package was called with invalid argument(s): ${PARSED_ARGS_UNPARSED_ARGUMENTS}") 
	endif()

    if(NOT PARSED_ARGS_PACKAGE_NAME)
        message(FATAL_ERROR "${CMAKE_PROJECT_NAME}/${PROJECT_NAME}: find_install_package called without a PACKAGE_NAME")
    endif()

	message(STATUS "${CMAKE_PROJECT_NAME}/${PROJECT_NAME}: finding package ${PARSED_ARGS_PACKAGE_NAME}")

	if(NOT PARSED_ARGS_VCPKG_NAME AND NOT PARSED_ARGS_CONAN_NAME AND NOT PARSED_ARGS_GIT_REPOSITORY)
		message(FATAL_ERROR "Not provided a valid vcpkg or conan package name or git repository")
	endif()
	
	# Set up some extra find_package hints
	# e.g.
	# project/lib/package
	# project/lib/extern/package
	# project/../lib/package
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

	# if it's a git repo, check for folders named after repo
	# since git can supply either a folder or .git repo, need to handle both cases		
	if (PARSED_ARGS_GIT_REPOSITORY)	
		# Get git archive name
		unset(GIT_ARCHIVE_NAME)
		unset(GIT_REPO_EXTENSION)
		get_filename_component(GIT_REPO_EXTENSION "${PARSED_ARGS_GIT_REPOSITORY}" EXT)
		if(GIT_REPO_EXTENSION)
			get_filename_component(GIT_ARCHIVE_NAME "${PARSED_ARGS_GIT_REPOSITORY}" NAME_WE)
		else()
			# turn directory into file to get final directory name (stupid way to do it but no better way until cmake 3.19?
			get_filename_component(GIT_FOLDER_NO_TRAILING "${PARSED_ARGS_GIT_REPOSITORY}" DIRECTORY)
			get_filename_component(GIT_ARCHIVE_NAME "${GIT_FOLDER_NO_TRAILING}.dummy" NAME_WE)	
		endif()
		
		list(APPEND EXTRA_PATH_HINTS "${PROJECT_SOURCE_DIR}/../lib/${GIT_ARCHIVE_NAME}/")
		list(APPEND EXTRA_PATH_HINTS "${PROJECT_SOURCE_DIR}/../lib/${GIT_ARCHIVE_NAME}/cmake/")
		list(APPEND EXTRA_PATH_HINTS "${PROJECT_SOURCE_DIR}/lib/${GIT_ARCHIVE_NAME}/")
		list(APPEND EXTRA_PATH_HINTS "${PROJECT_SOURCE_DIR}/lib/${GIT_ARCHIVE_NAME}/cmake/")
		list(APPEND EXTRA_PATH_HINTS "${PROJECT_SOURCE_DIR}/lib/extern/${GIT_ARCHIVE_NAME}/")
		list(APPEND EXTRA_PATH_HINTS "${PROJECT_SOURCE_DIR}/lib/extern/${GIT_ARCHIVE_NAME}/cmake/")
	endif()
	
	# Try to find existing package or install via vcpkg/conan
	if(PARSED_ARGS_SEARCH_HINTS)
		list(APPEND EXTRA_PATH_HINTS "${PARSED_ARGS_SEARCH_HINTS}")
		message(STATUS "Added hint to search path for ${PARSED_ARGS_PACKAGE_NAME}: ${PARSED_ARGS_SEARCH_HINTS}")
	endif()
	if(PARSED_ARGS_COMPONENTS)
		find_package(${PARSED_ARGS_PACKAGE_NAME} COMPONENTS ${PARSED_ARGS_COMPONENTS} HINTS ${EXTRA_PATH_HINTS} CONFIG) #removed required at this point, since if missing you want it to install rather than fail
	#else()
	#	find_package(${PARSED_ARGS_PACKAGE_NAME} HINTS ${EXTRA_PATH_HINTS} CONFIG) #removed required at this point, since if missing you want it to install rather than fail
	endif()
	
	# Install the package otherwise
	if(NOT ${PARSED_ARGS_PACKAGE_NAME}_FOUND)
		select_PREFERRED_PKG_MANAGER()

		# prefer vcpkg
		if(PREFERRED_PKG_MANAGER MATCHES "vcpkg")
			if (PARSED_ARGS_VCPKG_NAME AND (NOT PARSED_ARGS_VCPKG_NAME MATCHES "none")) # first choice vcpkg
				message(STATUS "Retrieving with first choice package manager (vcpkg)")
				install_package_vcpkg(${PARSED_ARGS_VCPKG_NAME})
				find_package(${PARSED_ARGS_PACKAGE_NAME} COMPONENTS ${PARSED_ARGS_COMPONENTS} HINTS ${EXTRA_PATH_HINTS} CONFIG)
				if(NOT ${PARSED_ARGS_PACKAGE_NAME}_FOUND)
					message(WARNING "Retrieval via vcpkg failed")
				endif()
			elseif(NOT ${PARSED_ARGS_PACKAGE_NAME}_FOUND AND PARSED_ARGS_CONAN_NAME AND (NOT PARSED_ARGS_CONAN_NAME MATCHES "none")) # second choice conan
				message(STATUS "Retrieving with second choice package manager (conan)")
				install_package_conan(${PARSED_ARGS_CONAN_NAME})
				find_package(${PARSED_ARGS_PACKAGE_NAME} COMPONENTS ${PARSED_ARGS_COMPONENTS} HINTS ${EXTRA_PATH_HINTS} CONFIG)
				if(NOT ${PARSED_ARGS_PACKAGE_NAME}_FOUND)
					message(WARNING "Retrieval via conan failed")
				endif()
			endif()
		elseif(PREFERRED_PKG_MANAGER MATCHES "conan")
			if(PARSED_ARGS_CONAN_NAME AND (NOT PARSED_ARGS_CONAN_NAME MATCHES "none")) # first choice conan
				message(STATUS "Retrieving with first choice package manager (conan)")
				install_package_conan(${PARSED_ARGS_CONAN_NAME})
				find_package(${PARSED_ARGS_PACKAGE_NAME} COMPONENTS ${PARSED_ARGS_COMPONENTS} HINTS ${EXTRA_PATH_HINTS} CONFIG)
				if(NOT ${PARSED_ARGS_PACKAGE_NAME}_FOUND)
					message(WARNING "Retrieval via vcpkg failed")
				endif()
			elseif(NOT ${PARSED_ARGS_PACKAGE_NAME}_FOUND AND PARSED_ARGS_VCPKG_NAME AND (NOT PARSED_ARGS_VCPKG_NAME MATCHES "none")) # second choice vcpkg
				message(STATUS "Retrieving with second choice package manager (vcpkg)")
				install_package_vcpkg(${PARSED_ARGS_VCPKG_NAME})
				find_package(${PARSED_ARGS_PACKAGE_NAME} COMPONENTS ${PARSED_ARGS_COMPONENTS} HINTS ${EXTRA_PATH_HINTS} CONFIG)
				if(NOT ${PARSED_ARGS_PACKAGE_NAME}_FOUND)
					message(WARNING "Retrieval via conan failed")
				endif()
			endif()
		elseif(NOT ${PARSED_ARGS_PACKAGE_NAME}_FOUND AND PARSED_ARGS_GIT_REPOSITORY AND (NOT PARSED_ARGS_GIT_REPOSITORY MATCHES "none"))
			message(STATUS "No conan or vcpkg package name supplied, trying git")
			install_package_git(${PARSED_ARGS_GIT_REPOSITORY})
			find_package(${PARSED_ARGS_PACKAGE_NAME} COMPONENTS ${PARSED_ARGS_COMPONENTS} HINTS ${EXTRA_PATH_HINTS} CONFIG)
			if(NOT ${PARSED_ARGS_PACKAGE_NAME}_FOUND)
				message(WARNING "Retrieval via git failed")
			endif()
		else()
			message(FATAL_ERROR "No valid package/repository supplied, should have been stopped at function start")
		endif()
	endif()
	
	if(${PARSED_ARGS_PACKAGE_NAME}_FOUND)
		message(STATUS "Found installed package: ${PARSED_ARGS_PACKAGE_NAME}")
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
	if(PARSED_ARGS_REQUIRED_TARGETS)
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
			
				# STATIC_LIBRARY, MODULE_LIBRARY, SHARED_LIBRARY, OBJECT_LIBRARY, INTERFACE_LIBRARY, EXECUTABLE
				get_target_property(type ${REQUIRED_TARGET} TYPE)

				# if (${REQUIRED_TARGET} STREQUAL "Corrade::Containers")
					# message("Corrade containers target type: ${type}")
				# endif()

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
	endif()
	
	if(TMP_PACKAGE_TARGETS)
		set(${PARSED_ARGS_PACKAGE_NAME}_TARGETS ${TMP_PACKAGE_TARGETS} CACHE INTERNAL "" FORCE)
		if(PRINT_INCLUDE_TARGET_DIRS)
			message(STATUS "${CMAKE_PROJECT_NAME}/${PROJECT_NAME}: Set ${PARSED_ARGS_PACKAGE_NAME}_TARGETS to ${${PARSED_ARGS_PACKAGE_NAME}_TARGETS}")
		endif()
	else()
		message(FATAL_ERROR "${CMAKE_PROJECT_NAME}/${PROJECT_NAME}: No targets found for package ${PARSED_ARGS_PACKAGE_NAME}")
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