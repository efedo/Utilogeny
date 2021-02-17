cmake_minimum_required(VERSION 3.10)

add_library(UtilogenyCore STATIC
    besthitmap.h
    commandcontroller/commandcontroller.cpp
    commandcontroller/commandcontroller.h
    commandcontroller/commandqueue.cpp
    commandcontroller/commandqueue.h
    commandcontroller/consolecontroller.cpp
    commandcontroller/consolecontroller.h
    commandcontroller/typedefs.h
    constable.h
    cpuinfo.cpp
    cpuinfo.h
    datafile.cpp
    datafile.h
    exceptions.cpp
    exceptions.h
    md5.cpp
    md5.h
    precomp.cpp
    precomp.h
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
 "platform.h")

target_precompile_headers(UtilogenyCore
	PRIVATE precomp.h)

set_target_properties(UtilogenyCore PROPERTIES
            CXX_STANDARD ${OVERRIDE_CXX_STANDARD}
            CXX_EXTENSIONS OFF
            )