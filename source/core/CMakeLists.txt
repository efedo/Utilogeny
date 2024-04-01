# Copyright 2021 Eric Fedosejevs
#

cmake_minimum_required(VERSION 3.15)

set(SOURCES
    besthitmap.h
    commandcontroller/commandcontroller.cpp
    commandcontroller/commandcontroller.h
    commandcontroller/commandqueue.cpp
    commandcontroller/commandqueue.h
    commandcontroller/consolecontroller.cpp
    commandcontroller/consolecontroller.h
    commandcontroller/typedefs.h
	console/termcolor.h
    constable.h
    cpuinfo.cpp
    cpuinfo.h
    datafile.cpp
    datafile.h
    exceptions.cpp
    exceptions.h
    md5.cpp
    md5.h
	platform.h
    precomp.cpp
    precomp.h
	#properties/property_container.cpp
	#properties/property.h
    randomnumbers.cpp
    randomnumbers.h
    re2/re2/bitmap256.h
    re2/re2/bitstate.cc
    re2/re2/compile.cc
    re2/re2/dfa.cc
    re2/re2/filtered_re2.cc
    re2/re2/filtered_re2.h
    re2/re2/mimics_pcre.cc
    re2/re2/nfa.cc
    re2/re2/onepass.cc
    re2/re2/parse.cc
    re2/re2/perl_groups.cc
    re2/re2/pod_array.h
    re2/re2/prefilter.cc
    re2/re2/prefilter.h
    re2/re2/prefilter_tree.cc
    re2/re2/prefilter_tree.h
    re2/re2/prog.cc
    re2/re2/prog.h
    re2/re2/re2.cc
    re2/re2/re2.h
    re2/re2/regexp.cc
    re2/re2/regexp.h
    re2/re2/set.cc
    re2/re2/set.h
    re2/re2/simplify.cc
    re2/re2/sparse_array.h
    re2/re2/sparse_set.h
    re2/re2/stringpiece.cc
    re2/re2/stringpiece.h
    re2/re2/tostring.cc
    re2/re2/unicode_casefold.cc
    re2/re2/unicode_casefold.h
    re2/re2/unicode_groups.cc
    re2/re2/unicode_groups.h
    re2/re2/walker-inl.h
    re2/util/logging.h
    re2/util/mix.h
    re2/util/mutex.h
    re2/util/rune.cc
    re2/util/strutil.cc
    re2/util/strutil.h
    re2/util/utf.h
    re2/util/util.h
    smarttimer.h
    threading.h
    utilities.cpp
    utilities.h
    utilogeny.cpp
    utilogeny.h
    version.h
	properties/property_old_variant.h
	properties/property_container.h
	properties/property_new_variant.h
	platform.cpp
)

if(UTILOGENY_AUDIO)
    include(${UTILOGENY_DIR}/cmake/target_add_sdl.cmake)
    include(${UTILOGENY_DIR}/cmake/target_add_soloud.cmake)
    set(SOURCES 
        ${SOURCES}
        "audio/audio_backend_generic.h"
        "audio/audio_backend_soloud.h"
        "audio/audio_backend_generic.h" 
        "audio/audio_backend_soloud.h"
    )
endif()

if(UTILOGENY_GTEST)
    include(${UTILOGENY_DIR}/lib/gtest/CMakeLists.txt)
    set(LINK_LIBS ${LINK_LIBS} "gtest_main")
    set(INCLUDE_DIRS ${INCLUDE_DIRS} ${googletest_SOURCE_DIR}/googletest/include)
endif()

if(UTILOGENY_TBB)
    include(${UTILOGENY_DIR}/lib/tbb/CMakeLists.txt)
    set(LINK_LIBS ${LINK_LIBS} "TBB::tbb")
    set(INCLUDE_DIRS ${INCLUDE_DIRS} ${tbb_SOURCE_DIR}/include)
endif()

if(UTILOGENY_SPDLOG)
    include(${UTILOGENY_DIR}/lib/spdlog/CMakeLists.txt)
    set(LINK_LIBS ${LINK_LIBS} "spdlog")
    set(INCLUDE_DIRS ${INCLUDE_DIRS} ${spdlog_SOURCE_DIR}/include)
endif()

add_library(UtilogenyCore STATIC ${SOURCES})

target_precompile_headers(UtilogenyCore
	PRIVATE precomp.h)
	
target_config_compiler(UtilogenyCore)

set_target_properties(UtilogenyCore PROPERTIES
            CXX_STANDARD ${OVERRIDE_CXX_STANDARD}
            CXX_EXTENSIONS OFF
            )

if(UTILOGENY_AUDIO)
    target_include_soloud(UtilogenyCore)
    target_include_sdl2(UtilogenyCore)
endif()

target_link_libraries(UtilogenyCore PUBLIC ${LINK_LIBS})
target_include_directories(UtilogenyCore PUBLIC ${INCLUDE_DIRS})
