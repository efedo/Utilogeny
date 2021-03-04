//
// pch.h
// Header for standard system include files.
//

#pragma once

#include "gtest/gtest.h"

// Pretty formatting for info messages in Gtest
#define GTEST_INFOMSG std::cerr << "\033[0;32m[----------] [ INFO ] \033[0m"

extern std::streambuf * const cerr_buffer_p;// = std::cerr.rdbuf();
void cerr_off();
void cerr_on();