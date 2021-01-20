// Copyright 2017-20 Eric Fedosejevs
//

#pragma once
#include "Utilogeny/source/core/precomp.h"
#include "Utilogeny/source/core/Utilogeny.h"
#include "Utilogeny/source/core/commandcontroller/consolecontroller.h"

typedef boost::iostreams::tee_device<std::ostream, std::ofstream> TeeDeviceInternal;
typedef boost::iostreams::stream<TeeDeviceInternal> TeeStreamInternal;

class TeeDevice {
public:
	TeeDeviceInternal device;
};

class TeeStream {
public:
	TeeStreamInternal stream;
};


// Log utilities
void prepTeeStream() {

	// Copy over old streams
	TeeDevice* oldOutputDeviceCout = Utilogeny::log::outputDeviceCout;
	TeeDevice* oldOutputDeviceCerr = Utilogeny::log::outputDeviceCerr;
	TeeStream* oldLoggerCout = Utilogeny::log::loggerCout;
	TeeStream* oldLoggerCerr = Utilogeny::log::loggerCerr;

	bool teeUpdateRequired = (Utilogeny::log::logFile != Utilogeny::log::teedLogFile);

	// If outputting to log file
	if (Utilogeny::log::logFile) {

		if (teeUpdateRequired) {
			// Prep new tee streams
			Utilogeny::log::outputDeviceCerr = new TeeDevice{ TeeDeviceInternal(Utilogeny::log::ostreamCacheCerr, *Utilogeny::log::logFile) };
			Utilogeny::log::loggerCerr = new TeeStream{ TeeStreamInternal(Utilogeny::log::outputDeviceCerr->device) };
			Utilogeny::log::outputDeviceCout = new TeeDevice{ TeeDeviceInternal(Utilogeny::log::ostreamCacheCout, *Utilogeny::log::logFile) };
			Utilogeny::log::loggerCout = new TeeStream{ TeeStreamInternal(Utilogeny::log::outputDeviceCout->device) };

			// Update teed log file
			Utilogeny::log::teedLogFile = Utilogeny::log::logFile;
		}

		// If also outputting to console
		if (!(Utilogeny::log::muteConsole)) {
			// Prep new cout tee stream
			std::cout.rdbuf(Utilogeny::log::loggerCout->stream.rdbuf());
		}
		// If outputting to log file only
		else {
			std::cout.rdbuf(Utilogeny::log::logFile->rdbuf());
		}

		// Prep new cerr tee stream
		std::cerr.rdbuf(Utilogeny::log::loggerCerr->stream.rdbuf());
	}

	// If outputting to console only (and not log file)
	else {
		if (Utilogeny::log::muteConsole) { // if console muted
			std::cout.rdbuf(0);
			std::cerr.rdbuf(Utilogeny::log::strmBufCacheCerr);
		}
		else { // if console not muted
			std::cout.rdbuf(Utilogeny::log::strmBufCacheCout);
			std::cerr.rdbuf(Utilogeny::log::strmBufCacheCerr);
		}

		// Clear teedlog file if it exists
		if (teeUpdateRequired) {
			// Prep new tee streams
			Utilogeny::log::outputDeviceCerr = 0;
			Utilogeny::log::loggerCerr = 0;
			Utilogeny::log::outputDeviceCout = 0;
			Utilogeny::log::loggerCout = 0;
			Utilogeny::log::teedLogFile = 0;
		}
	}

	// Delete old tee streams if necessary
	if (teeUpdateRequired) {
		if (oldOutputDeviceCout) delete oldOutputDeviceCout;
		if (oldOutputDeviceCerr) delete oldOutputDeviceCerr;
		if (oldLoggerCout) delete oldLoggerCout;
		if (oldLoggerCerr) delete oldLoggerCerr;
	}
}

void muteConsole(const tCommandNum& unmuteFollowing) {
	Utilogeny::log::muteConsole = true;
	Utilogeny::log::unmuteConsoleAfterCommand = unmuteFollowing;
	prepTeeStream();
}

void unMuteConsole() {
	Utilogeny::log::muteConsole = false;
	prepTeeStream();
}