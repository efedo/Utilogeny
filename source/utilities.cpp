// Copyright 2017-18 The Curators of the University of Missouri
//

//#include "source/core/stdafx.h"
//#include "source/core/globals.h"
#include "Utilogeny/source/precomp.h"
#include "Utilogeny/source/utilities.h"
#include "Utilogeny/source/exceptions.h"
//#include "source/core/project/project.h"

/// Checks a search string for characters that are not allowed and fixes it
bool checkFixRegexString(std::string & oldString) {
	std::string newString;

	bool allGoodChars = true;

	// Loop through characters of string
	bool reachedNonSpaceChar = false;
	for (std::string::const_iterator it = oldString.begin(); it != oldString.end(); ++it) {

		// These characters are allowed without escape
		if (std::isalnum(*it) || (*it == '=')) {
			newString.push_back(*it);
			reachedNonSpaceChar = true;
			continue;
		}

		// These characters are allowed, but should be escaped
		if ((*it == '.') || (*it == ',') || (*it == '-') || (*it == '_') ||
			(*it == '?') || (*it == '+') || (*it == '(') || (*it == ')') || (*it == '[') ||
			(*it == ']') || (*it == '<') || (*it == '>') || (*it == '{') || (*it == '}') ||
			(*it == '\"') || (*it == '\'')) {
			newString.push_back('\\');
			newString.push_back(*it);
			reachedNonSpaceChar = true;
			continue;
		}

		// Spaces require special handling
		if (*it == ' ') {
			if (!reachedNonSpaceChar) continue; // If it is a leading space, skip it
			newString.push_back(char('\\'));
			newString.push_back(char('s'));
			continue;
		}

		// If you got here you have a problem character that is being omitted
		allGoodChars = false;
	}

	oldString = newString;
	return allGoodChars;
}

// Determines if all characters in the string are alpha-numeric
bool isAllAlphanum(const std::string & tmpString) {
	for (std::string::const_iterator it = tmpString.begin(); it != tmpString.end(); ++it) {
		if (!std::isalnum(*it)) return false;
	}
	return true;
}

bool isAllNum(const std::string & tmpString) {
	for (std::string::const_iterator it = tmpString.begin(); it != tmpString.end(); ++it) {
		if (!std::isdigit(*it)) return false;
	}
	return true;
}

std::string doubleToString(const double & tmpDouble, const unsigned int & decPrecision) {

	// Screen out nans and infs
	if (!isfinite(tmpDouble)) return "nan/inf";

	std::stringstream stream;
	stream << std::fixed << std::setprecision(decPrecision) << tmpDouble;
	return stream.str();
}

unsigned int charToInt(char & tmpChar) {
	return static_cast<unsigned int>(tmpChar - '0');
}

// Faster implementation with clear
float _strToFloatSafe(const std::string& tmpString) {
	std::istringstream ss;
	ss.str(tmpString);
	ss.clear();
	float x = 0.0;
	ss >> x;
	return x;
}

// Faster implementation with clear
double _strToDoubSafe(const std::string & tmpString) {
	std::istringstream ss;
	ss.str(tmpString);
	ss.clear();
	double x = 0.0;
	ss >> x;
	return x;
}

// C code; probably not the safest
double strToDoub(const std::string & tmpString) {
	char const * p = tmpString.c_str();

	double r = 0.0;
	bool neg = false;
	if (*p == '-') {
		neg = true;
		++p;
	}
	while (*p >= '0' && *p <= '9') {
		r = (r*10.0) + (*p - '0');
		++p;
	}
	if (*p == '.') {
		double f = 0.0;
		int n = 0;
		++p;
		while (*p >= '0' && *p <= '9') {
			f = (f*10.0) + (*p - '0');
			++p;
			++n;
		}
		r += f / std::pow(10.0, n);
	}
	if (*p != 0) {
		try {
			return _strToDoubSafe(tmpString);
		}
		catch (...) {
			std::cerr << "Invalid character found in text string during conversion to double. Safe conversion function also failed." << std::endl;
			throw_line("Invalid character found in text string during conversion to double"); // Invalid character
		}
	}
	if (neg) {
		r = -r;
	}
	return r;
}

// C code; probably not the safest
float strToFloat(const std::string & tmpString) {
	char const * p = tmpString.c_str();

	float r = 0.0;
	bool neg = false;
	if (*p == '-') {
		neg = true;
		++p;
	}
	while (*p >= '0' && *p <= '9') {
		r = (r*10.0f) + (*p - '0');
		++p;
	}
	if (*p == '.') {
		float f = 0.0;
		int n = 0;
		++p;
		while (*p >= '0' && *p <= '9') {
			f = (f * 10.0f) + (*p - '0');
			++p;
			++n;
		}
		r += f / std::pow(10.0f, n);
	}
	if (*p != 0) {
		try {
			return _strToFloatSafe(tmpString);
		}
		catch (...) {
			std::cerr << "Invalid character found in text string during conversion to double. Safe conversion function also failed." << std::endl;
			throw_line("Invalid character found in text string during conversion to double"); // Invalid character
		}
	}
	if (neg) {
		r = -r;
	}
	return r;
}

//// Faster implementation with clear
//unsigned long strToULong(const std::string & tmpString) {
//	// Deprecated
//	// String stream conversions are SLOPPY
//	// e.g. incorrect conversion of negative integers, letters become numbers
//
//	//std::istringstream ss;
//	//ss.str(tmpString);
//	//ss.clear();
//	//unsigned long x = 0;
//	//ss >> x;
//	//return x;
//
//	throw; // SHOULD BE DEPRECATED
//}

unsigned long strToULongFast(const std::string & tmpString) {
	char const * p = tmpString.c_str();

	unsigned long r = 0;

	while (*p >= '0' && *p <= '9') {
		r = (r*10) + (*p - '0');
		++p;
	}

	if (*p != 0) {
		throw_line("Invalid character found in text string during conversion to unsigned long."); // Invalid character
	}

	return r;
}

bool strIsInt(const std::string & tmpString) {
	char const * p = tmpString.c_str();

	while (*p >= '0' && *p <= '9') {
		++p;
	}

	if (*p != 0) {
		return false;
	}

	return true;
}

bool strToBool(const std::string & tmpString) {
	if ((tmpString == "true") || (tmpString == "1")) {
		return true;
	}
	else if ((tmpString == "false") || (tmpString == "0")) {
		return false;
	}
	else {
		std::cerr << "Invalid boolean value supplied.\n";
		throw_line("Invalid boolean value supplied");
	}
}

// C code; should be fast
void splitLineOnTabs(const std::string & line, std::vector<std::string> & tokens, const size_t & _HintSz) {
	splitLineOnChar(line, '\t', tokens, _HintSz);
}

//void splitLineOnChar(std::vector<std::string *> & tokens, const std::string & line, char splitchar, bool keepEmpties) {
//	// Get C-str
//	char const * lineCstr = line.c_str();
//
//	// Clear tokens
//	tokens.clear();
//
//	// Loop through line cstr
//	unsigned long t = 0;
//	unsigned long c = 0;
//	char * curField = new char[200];
//	memset(curField, 0, 200); // Sets all of curField to null
//
//	while (1) {
//		char tmpChar = lineCstr[t];
//		if (tmpChar == 0) {
//			if (c || keepEmpties) {
//				tokens.push_back(new std::string); // provideKinEnergy back the string contents to the output buffer
//			}
//			break;
//		}
//		if (tmpChar == splitchar) { // if it's a tab...
//			if (c || keepEmpties) {
//				std::string * tmpStrPtr = new std::string;
//				tmpStrPtr->reserve(c + 1);
//				tmpStrPtr->assign(curField);
//				tokens.push_back(tmpStrPtr); // provideKinEnergy back the string contents to the output buffer
//				memset(curField, 0, 200); // reset the contents of the string to 0
//			}
//			c = 0; // indicate that the current string i
//		}
//		else {
//			if (c < 199) { // prevent buffer overflow; only add to string if less than 100 digits
//				curField[c] = tmpChar;
//			}
//			++c;
//		}
//		++t;
//	}
//}

inline void splitLineOnChar(const std::string & _Str, const char & _Tok, std::vector<std::string> & _Tokens, const size_t & _HintSz)
{
	_Tokens.reserve(_HintSz);

	const char* _Beg(&_Str[0]), *_End(&_Str[_Str.size()]);

	for (const char* _Ptr = _Beg; _Ptr < _End; ++_Ptr)
	{
		if (*_Ptr == _Tok)
		{
			_Tokens.push_back(std::string(_Beg, _Ptr));
			_Beg = 1 + _Ptr;
		}
	}

	_Tokens.push_back(std::string(_Beg, _End));
}

// C code; should be fast
std::vector<std::string> splitCommandLineFast(const std::string & line) {

	// Get C-str
	char const * lineCstr = line.c_str();

	// Vector of tokens
	std::vector<std::string> tokens;

	// Loop through line cstr
	unsigned long t = 0;
	unsigned long c = 0;
	bool inquote = false;
	std::unique_ptr<char []> curFieldCnt(new char[200]);
	char* curField = curFieldCnt.get();
	memset(curField, 0, 200); // Sets all of curField to null

	while (1) {
		char tmpChar = lineCstr[t];
		if ((tmpChar == 0) || ((tmpChar == '/') && (lineCstr[t+1] == '/') && (!inquote))) {
			if (c) {
				tokens.push_back(curField); // provide back the string contents to the output buffer
			}
			break;
		}
		if (tmpChar == '"') {
			inquote = !inquote;
			++t;
			continue;
		}
		if ((!inquote) && ((tmpChar == ' ')|| (tmpChar == '\t'))) { // if it's a tab... or a space
			if (c) {
				tokens.push_back(curField); // provide back the string contents to the output buffer
				memset(curField, 0, 100); // reset the contents of the string to 0
			}
			c = 0; // indicate that the current string i
		}
		else {
			if (c < 199) { // prevent buffer overflow; only add to string if less than 100 digits
				curField[c] = tmpChar;
			}
			++c;
		}
		++t;
	}
	return tokens;
}

//bool fileExists(const std::string & fileName) {
//	// Check if the file exists and if it does, ask whether to overwrite or not
//	if (boost::filesystem::exists(fileName)) {
//		return true;
//	}
//	return false;
//}

// Checks if a filename exists and if the file can be opened for output
void ensureSafeFilenameOverwrite(std::string & fileName) {
	while (true) {
		if (fileExists(fileName)) {
			std::cerr << "File " << fileName << " already exists! Overwrite? (Y/N)" << std::endl;
			while (true) {
				int choice = std::getchar();
				if ((choice == 'Y') || (choice == 'y')) {
					// Try opening the file for output
					std::ofstream testOut(fileName);
					if (!testOut.good()) {
						std::cerr << "File " << fileName << " failed to open!\n";
						std::cerr << "Please choose another file name." << std::endl;
						std::cin >> fileName;
						testOut.close();
						break;
					}
					else {
						testOut.close();
						return;
					}
				}
				else if ((choice == 'N') || (choice == 'n')) {
					std::cerr << "Please choose another file name." << std::endl;
					std::cin >> fileName;
					break;
				}
				else {
					std::cerr << "Invalid selection. Overwrite? (Y/N)" << std::endl;
				}
			}
		}
		else {
			return;
		}
	}
}

//void checkOutputFilename(std::string & outFile) {
//	// Checks if it is relative or absolute... if relative, add to project output directory
//	std::filesystem::path tmpPath(outFile);
//
//	if (tmpPath.is_relative()) {
//		if (!globals::projectPtr) throw_line("Project not loaded; cannot deduce project output path");
//		outFile = globals::projectPtr->getOutputDirectory() + outFile;
//	}
//}
//
//// Log utilities
//void prepTeeStream() {
//
//	// Copy over old streams
//	tTeeDevice * oldOutputDeviceCout = Utilogeny::log::outputDeviceCout;
//	tTeeDevice * oldOutputDeviceCerr = Utilogeny::log::outputDeviceCerr;
//	tTeeStream * oldLoggerCout = Utilogeny::log::loggerCout;
//	tTeeStream * oldLoggerCerr = Utilogeny::log::loggerCerr;
//
//	bool teeUpdateRequired = (Utilogeny::log::logFile != Utilogeny::log::teedLogFile);
//
//	// If outputting to log file
//	if (Utilogeny::log::logFile) {
//
//		if (teeUpdateRequired) {
//			// Prep new tee streams
//			Utilogeny::log::outputDeviceCerr = new tTeeDevice(Utilogeny::log::ostreamCacheCerr, *Utilogeny::log::logFile);
//			Utilogeny::log::loggerCerr = new tTeeStream(*Utilogeny::log::outputDeviceCerr);
//			Utilogeny::log::outputDeviceCout = new tTeeDevice(Utilogeny::log::ostreamCacheCout, *Utilogeny::log::logFile);
//			Utilogeny::log::loggerCout = new tTeeStream(*Utilogeny::log::outputDeviceCout);
//
//			// Update teed log file
//			Utilogeny::log::teedLogFile = Utilogeny::log::logFile;
//		}
//
//		// If also outputting to console
//		if (!(Utilogeny::log::muteConsole)) {
//			// Prep new cout tee stream
//			std::cout.rdbuf(Utilogeny::log::loggerCout->rdbuf());
//		}
//		// If outputting to log file only
//		else {
//			std::cout.rdbuf(Utilogeny::log::logFile->rdbuf());
//		}
//
//		// Prep new cerr tee stream
//		std::cerr.rdbuf(Utilogeny::log::loggerCerr->rdbuf());
//	}
//
//	// If outputting to console only (and not log file)
//	else {
//		if (Utilogeny::log::muteConsole) { // if console muted
//			std::cout.rdbuf(0);
//			std::cerr.rdbuf(Utilogeny::log::strmBufCacheCerr);
//		}
//		else { // if console not muted
//			std::cout.rdbuf(Utilogeny::log::strmBufCacheCout);
//			std::cerr.rdbuf(Utilogeny::log::strmBufCacheCerr);
//		}
//
//		// Clear teedlog file if it exists
//		if (teeUpdateRequired) {
//			// Prep new tee streams
//			Utilogeny::log::outputDeviceCerr = 0;
//			Utilogeny::log::loggerCerr = 0;
//			Utilogeny::log::outputDeviceCout = 0;
//			Utilogeny::log::loggerCout = 0;
//			Utilogeny::log::teedLogFile = 0;
//		}
//	}
//
//	// Delete old tee streams if necessary
//	if (teeUpdateRequired) {
//		if (oldOutputDeviceCout) delete oldOutputDeviceCout;
//		if (oldOutputDeviceCerr) delete oldOutputDeviceCerr;
//		if (oldLoggerCout) delete oldLoggerCout;
//		if (oldLoggerCerr) delete oldLoggerCerr;
//	}
//}
//
//void muteConsole(const tCommandNum & unmuteFollowing) {
//	Utilogeny::log::muteConsole = true;
//	Utilogeny::log::unmuteConsoleAfterCommand = unmuteFollowing;
//	prepTeeStream();
//}
//
//void unMuteConsole() {
//	Utilogeny::log::muteConsole = false;
//	prepTeeStream();
//}

// Slow-ish... C code would be faster
void replaceUnderscoresAndTrimSpaces(std::string & line) {

	// If it's empty, don't proceed
	if (line.empty()) return;

	std::string tmpNew;
	bool start = true;

	for (std::string::iterator it = line.begin(); it != line.end(); ++it) {
		if (start && ((*it == ' ') || (*it == '_'))) {
			continue;
		}
		else {
			start = false;
		}

		if (*it == '_') {
			tmpNew.push_back(' ');
		}
		else {
			tmpNew.push_back(*it);
		}
	}

	bool trimEnd = true;

	if (tmpNew.empty()) return; // If empty now, don't proceed

	while (trimEnd) {
		if (*(--tmpNew.end()) == ' ') {
			tmpNew.erase(--tmpNew.end());
		}
		else {
			trimEnd = false;
		}
	}
	line = tmpNew;
}

// Converts string to upper case
void toUpper(std::string & str) {
	std::locale loc;
	const std::string::size_type max = str.length();
	for (std::string::size_type i = 0; i < max; ++i) {
		str[i] = std::toupper(str[i], loc);
	}
}

// Converts string to lower case
void toLower(std::string & str) {
	std::locale loc;
	const std::string::size_type max = str.length();
	for (std::string::size_type i = 0; i < max; ++i) {
		str[i] = std::tolower(str[i], loc);
	}
}


std::vector<std::pair<unsigned long, unsigned long>> splitGroupIntoBins(const unsigned long groupSize, const unsigned int numBins) {

	if (!numBins) throw_line("Tried to divide a group into 0 bins (probably for thread distribution)");

	binBoundaryPairVector tmpPairVec;

	if (!groupSize) return tmpPairVec;

	// Split up gene list into desired number of sets
	unsigned int actNumBins = numBins;
	if (actNumBins > groupSize) { actNumBins = groupSize; } // Don't make more threads than there are genes
	const unsigned long highestNum = groupSize - 1;
	unsigned long genesPerChunk = highestNum / actNumBins;
	unsigned long min = 0;
	unsigned long max = genesPerChunk + (highestNum % actNumBins);

	for (unsigned int i = 0; i != actNumBins; ++i) {
		tmpPairVec.push_back(std::pair<unsigned long, unsigned long>(min, max));
		min = max + 1;
		max = min + genesPerChunk - 1;
	}
	return tmpPairVec;
}