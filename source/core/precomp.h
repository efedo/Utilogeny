#pragma once

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
#include <fstream>
#include <functional>
#include <initializer_list>
#include <intrin.h>
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

// Boost
#include <boost/filesystem.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/tee.hpp>

// Macros and headers required by re2
#ifndef WINVER
#define _WIN32_WINNT_WIN10 0x0A00 // Windows 10
#define WINVER _WIN32_WINNT_WIN10 // Windows 10
#endif //WINVER
#undef __APPLE__
#undef _POSIX_C_SOURCE
#if defined(__APPLE__)
#include <TargetConditionals.h> // Required by re2 for Apple support
#endif
#include <immintrin.h> // For re2, requires AVX2
#ifdef _MSC_VER
#include <intrin.h> // For re2
#endif

#define GTEST_INFOMSG std::cerr << "\033[0;32m[----------] [ INFO ] \033[0m"
