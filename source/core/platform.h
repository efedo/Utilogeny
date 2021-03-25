// Detects OS, architecture, and compiler settings

#pragma once

#include <cstdint>

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

constexpr int _ce_strcmp_internal(const char* a, const char* b);
constexpr int ce_strcmp(const char* a, const char* b);

// OS flags
// Linux and Linux - derived			__linux__
// Android								__ANDROID__(implies __linux__)
// Linux(non - Android)					__linux__ && !__ANDROID__
// Darwin(Mac OS X and iOS)				__APPLE__
// Windows								_WIN32
// Windows 64 bit						_WIN64(implies _WIN32)

// Check that CMake detected a supported OS
#if		(!defined OS_WINDOWS && !defined OS_MACOS && !defined OS_LINUX)
#error "CMake did not detect a supported OS!"
#endif

// Check that CMake-detected OS matches
#if		(defined OS_WINDOWS && (!defined _WIN32 || defined __APPLE__ || defined __linux__ || defined __ANDROID__)) || \
		(defined OS_MACOS   && (!defined __APPLE__ || defined _WIN32 || defined __linux__ || defined __ANDROID__)) || \
		(defined OS_LINUX   && (!defined __linux__ || defined _WIN32 || defined __APPLE__ || defined __ANDROID__))
#error "CMake/compiler OS mismatch!"
#endif

// Check that it is a 64-bit platform
#if !defined PLATFORM_64BIT
#error "Only 64 bit platforms supported!"
#endif

// Compiler flags
// Visual Studio			_MSC_VER
// gcc						__GNUC__
// clang					__clang__
// emscripten				__EMSCRIPTEN__(for asm.js and webassembly)
// MinGW 32					__MINGW32__
// MinGW - w64 32bit		__MINGW32__
// MinGW - w64 64bit		__MINGW64__
//  For detailed compiler tokens/macros, see here: https://sourceforge.net/p/predef/wiki/Compilers/

#if defined __MINGW32__ || defined __MINGW64__
#define PLATFORM_MINGW
#endif

#if (defined __EMSCRIPTEN__) && (!defined COMPILER_LINEAGE_EMSCRIPTEN)
#error "Emscripten compiler was undetected by CMake"
#endif

#if (!defined __EMSCRIPTEN__) && (defined COMPILER_LINEAGE_EMSCRIPTEN)
#error "CMake expected Emscripten compiler"
#endif

#if (defined __clang__) && (!defined COMPILER_LINEAGE_CLANG)
#error "Clang compiler was undetected by CMake"
#endif

#if (!defined __clang__) && (defined COMPILER_LINEAGE_CLANG)
#error "CMake expected Clang compiler"
#endif

#if (defined __GNUC__) && (!defined COMPILER_LINEAGE_GCC)
#error "GCC compiler was undetected by CMake"
#endif

#if (!defined __GNUC__) && (defined COMPILER_LINEAGE_GCC)
#error "CMake expected GCC compiler"
#endif

#if (defined _MSC_VER) && (!defined COMPILER_LINEAGE_MSVC)
#error "MSVC compiler was undetected by CMake"
#endif

#if (!defined _MSC_VER) && (defined COMPILER_LINEAGE_MSVC)
#error "CMake expected MSVC compiler"
#endif

// If apple_build_version flag is set, should be clang and AppleClang
#if (defined __apple_build_version__) && (!defined COMPILER_LINEAGE_CLANG)
#error "__apple_build_version__ defined but not AppleClang?"
#endif

// Not all LLVM-based compilers define __llvm__, but all compilers with __llvm__ should be LLVM-based
#if (defined __llvm__) && (!defined COMPILER_LINEAGE_LLVM)
#error "CMake did not expect an LLVM-based compiler"
#endif