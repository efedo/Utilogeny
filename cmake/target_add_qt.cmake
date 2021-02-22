include(${UTILOGENY_DIR}/cmake/target_deploy_lib.cmake)

function(target_link_qt target)

	find_package(Qt MODULE)
	
	# Since find_package is nested in a function, need to promote
	if(NOT QT_VERSION_MAJOR)
		message(FATAL_ERROR "Should have Qt version from find package by now")
	endif()
	
	message(STATUS "Linking Qt to ${target}")

	target_include_directories(${target} PRIVATE ${QT5_INCLUDE_DIRS})
	target_link_libraries(${target} ${QT5_TARGETS})

	# Install required DLLs
	if(OS_WINDOWS2) # If Windows, might as well use WinDeployQt
	
	 	set(CMAKE_INSTALL_UCRT_LIBRARIES TRUE) # installs UCRT libraries when installing system libraries (below)
	
		get_target_property(_qmake_executable Qt5::qmake IMPORTED_LOCATION)
		get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
		find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${_qt_bin_dir}")
		if (NOT WINDEPLOYQT_EXECUTABLE)
			message(FATAL_ERROR "WinDeployQt not found")
		endif()
		
		# Qt file installation during build
		add_custom_command(TARGET ${target} POST_BUILD
			COMMAND ${WINDEPLOYQT_EXECUTABLE} $<TARGET_FILE:${target}> --verbose 2
			COMMENT "Deploying Qt via WinDeployQt (post-build)..."
		)
		
		# Qt file installation during installation
		set(QT_TARGET_EXE_PATH ${CMAKE_INSTALL_PREFIX}/bin/${target}.exe)	
		install(CODE "
			message(\"Deploying Qt via WinDeployQt (during installation)...\")		
			execute_process(
				COMMAND ${WINDEPLOYQT_EXECUTABLE} \"${QT_TARGET_EXE_PATH}\" --verbose 2
			)
		")
		
		message(STATUS "Windows Qt installation successfully set up for target ${target} via WinDeployQt.exe")
		
	elseif(OS_APPLE2) # If Apple, might as well use MacDeployQt
		get_target_property(_qmake_executable Qt5::qmake IMPORTED_LOCATION)
		get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
		find_program(MACDEPLOYQT_EXECUTABLE macdeployqt HINTS "${_qt_bin_dir}")
		if (NOT MACDEPLOYQT_EXECUTABLE)
			message(FATAL_ERROR "MacDeployQt not found")
		endif()
		
		# Qt file installation during build
		add_custom_command(TARGET ${target} POST_BUILD
			COMMAND ${MACDEPLOYQT_EXECUTABLE} $<TARGET_FILE:${target}> -always-overwrite
			COMMENT "Deploying Qt via MacDeployQt..."
		)

		# Qt file installation during installation
		set(QT_TARGET_EXE_PATH ${CMAKE_INSTALL_PREFIX}/bin/$<TARGET_FILE:${target}>)
		install(CODE "
			execute_process(
				COMMAND ${MACDEPLOYQT_EXECUTABLE} ${QT_TARGET_EXE_PATH} -always-overwrite
				COMMENT \"Deploying Qt via MacDeployQt...\"
			)
		")
		message(STATUS "MacOS Qt installation successfully set up for target ${target} via MacDeployQt.exe")
		
	else()
	
		# https://github.com/probonopd/linuxdeployqt
	
		message(STATUS "Deploying Qt manually (should not occur on Windows or MacOS). Good luck!")
		foreach(QT_LIBRARIES_REQUIRED ${QT_LIBRARIES_REQUIRED})
		
			target_deploy_libraries(${target} ${QT_LIBRARIES_REQUIRED})
					
			# # Qt file library deployment during build
			# add_custom_command(TARGET ${target} POST_BUILD
				# COMMAND ${CMAKE_COMMAND} -E copy_if_different \"$<TARGET_FILE:Qt5::${QT_LIBRARIES_REQUIRED}>\" \"$<TARGET_FILE_DIR:${target}>\"
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
		message(STATUS "Non-Windows post-build installation set up for target ${target} (should work for most platforms)")
	endif()
	
	# Theoretically automoc should take care of this, but just in case....
	set_property(TARGET ${target} PROPERTY AUTOMOC TRUE)
	set_property(TARGET ${target} PROPERTY AUTOUIC TRUE)
	set_property(TARGET ${target} PROPERTY AUTORCC TRUE)

endfunction()