// Utilogeny.cpp : Defines the functions for the static library.
//
#include "Utilogeny/source/core/precomp.h"
#include "Utilogeny/source/core/Utilogeny.h"

// Log file stuff
namespace Utilogeny {
	namespace log {
		std::ostream ostreamCacheCout(std::cout.rdbuf());
		std::ostream ostreamCacheCerr(std::cerr.rdbuf());
		std::streambuf* strmBufCacheCout = std::cout.rdbuf();
		std::streambuf* strmBufCacheCerr = std::cerr.rdbuf();
		tTeeDevice* outputDeviceCout = 0;
		tTeeDevice* outputDeviceCerr = 0;
		tTeeStream* loggerCout = 0;
		tTeeStream* loggerCerr = 0;
		std::ofstream* logFile = 0;
		std::ofstream* teedLogFile = 0;
		bool muteConsole = false;
		tCommandNum unmuteConsoleAfterCommand = 0;
	}

	// Output mutexes
	std::recursive_mutex consoleOutMutex;
}

// TODO: This is an example of a library function
void fnUtilogeny()
{
}
