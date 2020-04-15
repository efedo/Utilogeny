// Copyright 2017-18 The Curators of the University of Missouri
//

#pragma once
#include "Utilogeny/source/precomp.h"
//#include "source/core/globals.h"

typedef uint32_t tCommandNum; // Note also needs to be defined in 
enum class tCommandSource : uint8_t { GUI = 1, console = 2, script = 3 };
typedef boost::iostreams::tee_device<std::ostream, std::ofstream> tTeeDevice;
typedef boost::iostreams::stream<tTeeDevice> tTeeStream;