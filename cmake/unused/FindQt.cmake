include(FindPackageHandleStandardArgs)

#if(NOT QT_FOUND)

# Qt prefix
set(QT_PREFIX "C:/Qt/5.15.1/msvc2019_64")
set(CMAKE_PREFIX_PATH ${QT_PREFIX})

## Note: there is a bug with find_package for Qt5.15. The below does not always correctly retrieve requested components, and fails silently
## Manually loading individual packages as well below to get around this
find_package(QT5 REQUIRED COMPONENTS $QT_LIBRARIES_REQUIRED)

# Set global (cache) variables needed by UIC and MOC
if(QT5_FOUND)
	message(STATUS "Found QT5 version: ${QT5_VERSION_MAJOR}.${QT5_VERSION_MINOR}")
	set(QT_VERSION_MAJOR ${QT5_VERSION_MAJOR} CACHE STRING "Qt Major version" FORCE)
	set(QT_VERSION_MINOR ${QT5_VERSION_MINOR} CACHE STRING "Qt Minor version" FORCE)
	set(QT5_VERSION_MAJOR ${QT5_VERSION_MAJOR} CACHE STRING "Qt5 Major version" FORCE)
	set(QT5_VERSION_MINOR ${QT5_VERSION_MINOR} CACHE STRING "Qt5 Minor version" FORCE)
else()
	message(FATAL_ERROR "QT5 not found")
endif()

set(QT_FOUND TRUE)

#endif()

# Find the individual Qt libraries (in case find package above failed for one or more components)
unset(QT5_INCLUDE_DIRS)
unset(QT5_TARGETS)
foreach(QT_LIBRARIES_REQUIRED ${QT_LIBRARIES_REQUIRED})
	find_package( Qt5${QT_LIBRARIES_REQUIRED} REQUIRED )
	list(APPEND QT5_INCLUDE_DIRS ${Qt5${QT_LIBRARIES_REQUIRED}_INCLUDE_DIRS})
	list(APPEND QT5_TARGETS Qt5::${QT_LIBRARIES_REQUIRED})		
	if ( Qt5${QT_LIBRARIES_REQUIRED}_FOUND )
		message(STATUS "  Qt5${QT_LIBRARIES_REQUIRED} found")
	else()
		message(FATAL_ERROR "  Qt5${QT_LIBRARIES_REQUIRED} not found")
	endif()
endforeach()

# Setup Qt uic, moc, and rcc
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)