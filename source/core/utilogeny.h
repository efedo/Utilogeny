#pragma once

#include "Utilogeny/source/core/precomp.h"
#include "Utilogeny/source/core/commandcontroller/typedefs.h"

class TeeDevice;
class TeeStream;

// Log file stuff
namespace Utilogeny {
	namespace log {
		extern std::ostream ostreamCacheCout, ostreamCacheCerr;
		extern std::streambuf* strmBufCacheCout;
		extern std::streambuf* strmBufCacheCerr;
		extern TeeDevice* outputDeviceCout;
		extern TeeDevice* outputDeviceCerr;
		extern TeeStream* loggerCout;
		extern TeeStream* loggerCerr;
		extern std::ofstream* logFile;
		extern std::ofstream* teedLogFile;
		extern bool muteConsole;
		extern tCommandNum unmuteConsoleAfterCommand;
	}

	// Output mutexes
	extern std::recursive_mutex consoleOutMutex;

	constexpr float mask_flt = -INFINITY;// (1e+300 * 1e+300); // as a reminder that float-float comparisons are SLOW compared to bitwise comparisons
}