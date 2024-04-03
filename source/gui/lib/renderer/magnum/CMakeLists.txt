cmake_minimum_required(VERSION 3.14)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(AbstractorizerMagnum_SOURCES 
	magnum_widget.cpp
    magnum_widget.h
	magnum_widget_internal.h
	magnum_widget_internal.cpp
)

if(INCLUDE_SDL)
	set(AbstractorizerMagnum_SOURCES ${AbstractorizerMagnum_SOURCES}
		
		
	)
endif()

add_library(AbstractorizerMagnum
	${AbstractorizerMagnum_SOURCES}
)

set_target_properties(AbstractorizerMagnum PROPERTIES
    CXX_STANDARD 17
    CXX_EXTENSIONS OFF
)

target_compile_options(AbstractorizerMagnum PUBLIC "/Zc:__cplusplus")

if(INCLUDE_SDL)
	target_link_sdl2(AbstractorizerMagnum)
endif()

target_include_qt_or_cs(AbstractorizerMagnum)
target_include_magnum(TARGET AbstractorizerMagnum CORRADE_COMPONENTS "${CORRADE_LIBRARIES_REQUIRED}" MAGNUM_COMPONENTS "${MAGNUM_LIBRARIES_REQUIRED}")