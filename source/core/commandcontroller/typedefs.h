// Copyright 2017-20 Eric Fedosejevs
//

#pragma once
#include "Utilogeny/source/core/precomp.h"
//#include "source/core/globals.h"

typedef uint32_t tCommandNum; // Note also needs to be defined in 
enum class tCommandSource : uint8_t { GUI = 1, console = 2, script = 3 };