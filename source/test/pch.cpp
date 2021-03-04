//
// pch.cpp
// Include the standard header and generate the precompiled header.
//

#include "pch.h"

std::streambuf * const cerr_buffer_p = std::cerr.rdbuf();

void cerr_off() {

    std::cerr.rdbuf(nullptr);
}

void cerr_on() {
    std::cerr.rdbuf(cerr_buffer_p);
}