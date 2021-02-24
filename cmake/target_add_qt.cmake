# Copyright 2021 Eric Fedosejevs
#

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

	unset(QT5_REQUIRED_TARGETS)
	foreach(REQUIRED_COMPONENT ${PARSED_ARGS_COMPONENTS})
		list(APPEND QT5_REQUIRED_TARGETS "Qt5::${REQUIRED_COMPONENT}")
	endforeach()

	find_install_package(PACKAGE_NAME "Qt5" COMPONENTS "${PARSED_ARGS_COMPONENTS}" REQUIRED_TARGETS "${QT5_REQUIRED_TARGETS}")
	target_include_lib(LINK_TARGET ${PARSED_ARGS_TARGET} PACKAGE_NAME "Qt5")
	
	# Theoretically automoc should take care of this, but just in case....
	set_property(TARGET ${PARSED_ARGS_TARGET} PROPERTY AUTOMOC TRUE)
	set_property(TARGET ${PARSED_ARGS_TARGET} PROPERTY AUTOUIC TRUE)
	set_property(TARGET ${PARSED_ARGS_TARGET} PROPERTY AUTORCC TRUE)
	
	#target_include_directories(${PARSED_ARGS_TARGET} PRIVATE ${QT5_INCLUDE_DIRS})
	
	#Extra Qt include dirs since Qt CMake target config is rather buggered
	
	# set(THIS_TARGET_INCLUDE_DIRS ${QT5_INCLUDE_DIRS})
	# list(APPEND THIS_TARGET_INCLUDE_DIRS ${QT5_INCLUDE_DIR})	
	# foreach(QT_LIBRARIES_REQUIRED ${QT_LIBRARIES_REQUIRED})
		# if(NOT Qt5${QT_LIBRARIES_REQUIRED}_INCLUDE_DIRS)
			# message(FATAL_ERROR "Qt5 include dir not supplied: Qt5${QT_LIBRARIES_REQUIRED}_INCLUDE_DIRS")
		# endif()
		# list(APPEND THIS_TARGET_INCLUDE_DIRS ${Qt5${QT_LIBRARIES_REQUIRED}_INCLUDE_DIRS})		
		# add_dependencies(${PARSED_ARGS_TARGET} "Qt5::${QT_LIBRARIES_REQUIRED}")		
	# endforeach()
	# target_include_directories(${PARSED_ARGS_TARGET} PRIVATE ${THIS_TARGET_INCLUDE_DIRS})
	# message(STATUS "Adding Qt include dependencies to ${PARSED_ARGS_TARGET}: ${THIS_TARGET_INCLUDE_DIRS}")	
	
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
		list(APPEND QT5_TARGETS "Qt5::${REQUIRED_COMPONENT}")
	endforeach()
	
	target_include_qt(TARGET ${PARSED_ARGS_TARGET} COMPONENTS ${PARSED_ARGS_COMPONENTS})
	target_link_deploy_lib(LINK_TARGET ${PARSED_ARGS_TARGET} PACKAGE_NAME "Qt5")
	
	# Theoretically automoc should take care of this, but just in case....
	set_property(TARGET ${PARSED_ARGS_TARGET} PROPERTY AUTOMOC TRUE)
	set_property(TARGET ${PARSED_ARGS_TARGET} PROPERTY AUTOUIC TRUE)
	set_property(TARGET ${PARSED_ARGS_TARGET} PROPERTY AUTORCC TRUE)
	
	# Install required DLLs
	if(CMAKE_OS MATCHES "WINDOWS") # If Windows, might as well use WinDeployQt
	
	 	set(CMAKE_INSTALL_UCRT_LIBRARIES TRUE) # installs UCRT libraries when installing system libraries (below)
	
		get_target_property(_qmake_executable Qt5::qmake IMPORTED_LOCATION)
		get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
		find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${_qt_bin_dir}")
		if (NOT WINDEPLOYQT_EXECUTABLE)
			message(FATAL_ERROR "WinDeployQt not found")
		endif()
		
		# Qt file installation during build
		add_custom_command(TARGET ${PARSED_ARGS_TARGET} POST_BUILD
			COMMAND ${WINDEPLOYQT_EXECUTABLE} $<TARGET_FILE:${PARSED_ARGS_TARGET}> --verbose 2
			COMMENT "Deploying Qt via WinDeployQt (post-build)..."
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
		get_target_property(_qmake_executable Qt5::qmake IMPORTED_LOCATION)
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
		foreach(QT_LIBRARIES_REQUIRED ${QT_LIBRARIES_REQUIRED})
			target_link_deploy_lib(TARGET ${PARSED_ARGS_TARGET} LINK_TARGET "Qt5::${QT_LIBRARIES_REQUIRED}")
				
			# # Qt file library deployment during build
			# add_custom_command(TARGET ${PARSED_ARGS_TARGET} POST_BUILD
				# COMMAND ${CMAKE_COMMAND} -E copy_if_different \"$<TARGET_FILE:Qt5::${QT_LIBRARIES_REQUIRED}>\" \"$<TARGET_FILE_DIR:${PARSED_ARGS_TARGET}>\"
				# COMMENT "Deploying Qt manually..."
			# )
			
			# # Qt file library deployment during installation
			# install(CODE "
				# execute_process(
					# COMMAND ${CMAKE_COMMAND} -E copy_if_different \"$<TARGET_FILE:Qt5::${QT_LIBRARIES_REQUIRED}>\" \"${CMAKE_INSTALL_PREFIX}/bin>\"
					# COMMENT \"Deploying Qt manually...\"
				# )
			# ")
			
		endforeach()
		message(STATUS "Non-Windows post-build installation set up for target ${PARSED_ARGS_TARGET} (should work for most platforms)")
	endif()	
endmacro()