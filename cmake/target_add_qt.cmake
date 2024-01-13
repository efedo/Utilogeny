# Copyright 2021-2023 Eric Fedosejevs
# Version 2023-12-11
# - Removed vcpkg retrieval and added qt_standard_project_setup()

include(${UTILOGENY_DIR}/cmake/find_install_package.cmake)
include(${UTILOGENY_DIR}/cmake/target_deploy_lib.cmake)

# These need to be macros unfortunately because Qt sets various global variables required
# by automoc/rcc/ui
macro(target_include_qt)

    cmake_parse_arguments(
        PARSED_ARGS # prefix of output variables
        "" # list of names of the boolean arguments (only defined ones will be true)
        "TARGET" # list of names of mono-valued arguments
        "COMPONENTS" # list of names of multi-valued arguments (output variables are lists)
        ${ARGN} # arguments of the function to parse, here we take the all original ones
    )
	
	if((NOT PARSED_ARGS_TARGET) OR (NOT PARSED_ARGS_COMPONENTS))
		message(FATAL_ERROR "target_include_qt() or target_link_qt() not supplied valid arguments")
	endif()

	unset(QT6_REQUIRED_TARGETS)
	foreach(REQUIRED_COMPONENT ${PARSED_ARGS_COMPONENTS})
		list(APPEND QT6_REQUIRED_TARGETS "Qt6::${REQUIRED_COMPONENT}")
	endforeach()

	find_install_package(PACKAGE_NAME "Qt6" COMPONENTS "${PARSED_ARGS_COMPONENTS}" REQUIRED_TARGETS "${QT6_REQUIRED_TARGETS}")
	qt_standard_project_setup() # qt6 addition

	target_include_lib(LINK_TARGET ${PARSED_ARGS_TARGET} PACKAGE_NAME "Qt6")
	
	# Theoretically automoc should take care of this, but just in case....
	set_property(TARGET ${PARSED_ARGS_TARGET} PROPERTY AUTOMOC TRUE)
	set_property(TARGET ${PARSED_ARGS_TARGET} PROPERTY AUTOUIC TRUE)
	set_property(TARGET ${PARSED_ARGS_TARGET} PROPERTY AUTORCC TRUE)	
endmacro()

macro(target_link_qt)

    cmake_parse_arguments(
        PARSED_ARGS # prefix of output variables
        "" # list of names of the boolean arguments (only defined ones will be true)
        "TARGET" # list of names of mono-valued arguments
        "COMPONENTS" # list of names of multi-valued arguments (output variables are lists)
        ${ARGN} # arguments of the function to parse, here we take the all original ones
    )
	
	foreach(REQUIRED_COMPONENT ${PARSED_ARGS_COMPONENTS})
		list(APPEND QT6_TARGETS "Qt6::${REQUIRED_COMPONENT}")
	endforeach()
	
	target_include_qt(TARGET ${PARSED_ARGS_TARGET} COMPONENTS ${PARSED_ARGS_COMPONENTS})
	target_link_deploy_lib(LINK_TARGET ${PARSED_ARGS_TARGET} PACKAGE_NAME "Qt6")
	
	# Theoretically automoc should take care of this, but just in case....
	set_property(TARGET ${PARSED_ARGS_TARGET} PROPERTY AUTOMOC TRUE)
	set_property(TARGET ${PARSED_ARGS_TARGET} PROPERTY AUTOUIC TRUE)
	set_property(TARGET ${PARSED_ARGS_TARGET} PROPERTY AUTORCC TRUE)
	
	# Install required DLLs
	if(CMAKE_OS MATCHES "WINDOWS") # If Windows, might as well use WinDeployQt
	
	 	set(CMAKE_INSTALL_UCRT_LIBRARIES TRUE) # installs UCRT libraries when installing system libraries (below)
	
		get_target_property(_qmake_executable Qt6::qmake IMPORTED_LOCATION)
		get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
		message(STATUS "Qt bin dir: ${_qt_bin_dir}")
		#find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${_qt_bin_dir}")
		set(WINDEPLOYQT_EXECUTABLE "${_qt_bin_dir}/windeployqt6.exe")
		if (WINDEPLOYQT_EXECUTABLE)
			message(STATUS "WinDeployQt found: ${WINDEPLOYQT_EXECUTABLE}")
		else()
			message(FATAL_ERROR "WinDeployQt not found")
		endif()
		
		# Qt file installation during build
		add_custom_command(TARGET ${PARSED_ARGS_TARGET} POST_BUILD
			COMMENT "Deploying Qt via WinDeployQt (post-build)..."
			#--qmake "${_qmake_executable}" 
			COMMAND ${WINDEPLOYQT_EXECUTABLE} $<TARGET_FILE:${PARSED_ARGS_TARGET}> --verbose 2
		)
		
		# Qt file installation during installation
		set(QT_TARGET_EXE_PATH ${CMAKE_INSTALL_PREFIX}/bin/${PARSED_ARGS_TARGET}.exe)	
		install(CODE "
			message(\"Deploying Qt via WinDeployQt (during installation)...\")		
			execute_process(
				COMMAND ${WINDEPLOYQT_EXECUTABLE} \"${QT_TARGET_EXE_PATH}\" --verbose 2
			)
		")
		
		message(STATUS "Windows Qt installation successfully set up for target ${PARSED_ARGS_TARGET} via WinDeployQt.exe")
		
	elseif(CMAKE_OS MATCHES "MACOS") # If Apple, might as well use MacDeployQt
		get_target_property(_qmake_executable Qt6::qmake IMPORTED_LOCATION)
		get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
		find_program(MACDEPLOYQT_EXECUTABLE macdeployqt HINTS "${_qt_bin_dir}")
		if (NOT MACDEPLOYQT_EXECUTABLE)
			message(FATAL_ERROR "MacDeployQt not found")
		endif()
		
		# Qt file installation during build
		add_custom_command(TARGET ${PARSED_ARGS_TARGET} POST_BUILD
			COMMAND ${MACDEPLOYQT_EXECUTABLE} $<TARGET_FILE:${PARSED_ARGS_TARGET}> -always-overwrite
			COMMENT "Deploying Qt via MacDeployQt..."
		)

		# Qt file installation during installation
		set(QT_TARGET_EXE_PATH ${CMAKE_INSTALL_PREFIX}/bin/$<TARGET_FILE:${PARSED_ARGS_TARGET}>)
		install(CODE "
			execute_process(
				COMMAND ${MACDEPLOYQT_EXECUTABLE} ${QT_TARGET_EXE_PATH} -always-overwrite
				COMMENT \"Deploying Qt via MacDeployQt...\"
			)
		")
		message(STATUS "MacOS Qt installation successfully set up for target ${PARSED_ARGS_TARGET} via MacDeployQt.exe")
	else()
		# https://github.com/probonopd/linuxdeployqt
	
		message(STATUS "Deploying Qt manually (should not occur on Windows or MacOS). Good luck!")
		foreach(LIBRARY_REQUIRED ${PARSED_ARGS_COMPONENTS})
			target_link_deploy_lib(LINK_TARGET ${PARSED_ARGS_TARGET} PACKAGE_TARGETS "Qt6::${LIBRARY_REQUIRED}")			
		endforeach()
		message(STATUS "Non-Windows post-build installation set up for target ${PARSED_ARGS_TARGET} (should work for most platforms)")
	endif()	
endmacro()