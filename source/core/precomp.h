#pragma once

#define NOMINMAX // Pre-emptively prevent min/max macro pollution in case windows.h gets included somewhere

// Standard library
#include <algorithm>
#include <array>
#include <assert.h>
#include <atomic>
#include <bitset>
#include <chrono>
#include <climits>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <ctype.h>
#include <deque>
#include <errno.h>
#include <filesystem>
#include <fstream>
#include <functional>
#include <initializer_list>
#include <iomanip>
#include <iosfwd>
#include <iostream>
#include <iterator>
#include <limits>
#include <list>
#include <locale>
#include <map>
#include <memory>
#include <mutex>
#include <new>
#include <numeric>
#include <ostream>
#include <queue>
#include <random>
#include <set>
#include <sstream>
#include <stack>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string_view>
#include <string>
#include <thread>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

// Boost includes

#include "Utilogeny/lib/boost/algorithm/string.hpp"
#include "Utilogeny/lib/boost/iostreams/stream.hpp"
#include "Utilogeny/lib/boost/iostreams/tee.hpp"
#include "Utilogeny/lib/boost/math/distributions/fisher_f.hpp"
#include "Utilogeny/lib/boost/math/distributions/students_t.hpp"
#include "Utilogeny/lib/boost/multiprecision/cpp_bin_float.hpp"

// Platform header
#include "Utilogeny/source/core/platform.h"

// Macros and headers required by re2
#ifndef WINVER
#define _WIN32_WINNT_WIN10 0x0A00 // Windows 10
#define WINVER _WIN32_WINNT_WIN10 // Windows 10
#endif //WINVER

#ifdef _DEBUG // For MSVC
#define DEBUG
#endif

#ifdef OS_MACOS
#include <TargetConditionals.h> // Required by re2 for Apple support
#endif

// Intrinsics
#include <immintrin.h> // For re2, requires AVX2
#ifdef COMPILER_MSVC
#include <intrin.h> // For re2
#elif defined(COMPILER_GCC) || defined(COMPILER_CLANG) || defined(COMPILER_LLVM)
#include "cpuid.h"
#endif