#pragma once

#include "Utilogeny/source/core/precomp.h"
#include "Utilogeny/source/core/commandcontroller/typedefs.h"

// Log file stuff
namespace Utilogeny {
	namespace log {
		extern std::ostream ostreamCacheCout, ostreamCacheCerr;
		extern std::streambuf* strmBufCacheCout;
		extern std::streambuf* strmBufCacheCerr;
		extern tTeeDevice* outputDeviceCout;
		extern tTeeDevice* outputDeviceCerr;
		extern tTeeStream* loggerCout;
		extern tTeeStream* loggerCerr;
		extern std::ofstream* logFile;
		extern std::ofstream* teedLogFile;
		extern bool muteConsole;
		extern tCommandNum unmuteConsoleAfterCommand;
	}

	// Output mutexes
	extern std::recursive_mutex consoleOutMutex;
}