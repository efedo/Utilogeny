// Copyright 2017-20 Eric Fedosejevs
//

#include "Utilogeny/source/core/precomp.h"
#include "Utilogeny/source/core/datafile.h"
#include "Utilogeny/source/core/exceptions.h"

// Section labels

void cDataFile::checkStr(const std::string & labelString, const std::string & err) {

	// Make a modifiable string

	std::string tmpLabelString = labelString;

	// Print or get string
	procStr(tmpLabelString);

	// Check if it matched
	if (!_isWriting) {

		// Check if equivalent
		if (tmpLabelString != labelString) {
			std::string errStr = err + "\n" + "Expected: \"" + labelString + "\". Instead read: \"" + tmpLabelString + "\"";
			throw_line(errStr);
		}
	}
}

void cDataFile::_procBytes(char * varPtr, const std::size_t & size) {
	if (_isWriting) {
		_printBytes(varPtr, size);
	}
	else {
		_getBytes(varPtr, size);
	}
}


void cDataFile::_getBytes(char * varPtr, const std::size_t & size) {
	fstreamPtr->read(varPtr, size);
	if (fstreamPtr->fail()) throw_line("File read failed");
}

// Print variable
void cDataFile::_printBytes(const char * varPtr, const std::size_t & size) {
	fstreamPtr->write(varPtr, size);
	if (fstreamPtr->fail()) throw_line("File write failed");
}

bool cDataFileMonolithic::_openFileRead(const std::string& fileName) {
	return openFile(fileName, std::ios::binary | std::ios::in);
}

bool cDataFileMonolithic::_newFileWrite(const std::string& fileName) {
	return openFile(fileName, std::ios::binary | std::ios::out | std::ios::trunc);
}

bool cDataFileRandomAccess::openFileReadWrite(const std::string& fileName) {
	return openFile(fileName, std::fstream::binary | std::fstream::in | std::fstream::out);
}

bool cDataFileRandomAccess::newFileReadWrite(const std::string& fileName) {
	return openFile(fileName, std::fstream::binary | std::fstream::in | std::fstream::out | std::fstream::trunc);
}

bool cDataFile::openFile(const std::string& fileName, std::ios::openmode mode) {
	const unsigned long bufSize = 1048576; // Set file buffer size to 1 mb
	fileBufferPtr.reset(new char[bufSize]);
	fstreamPtr.reset(new std::fstream(fileName, mode));
	fstreamPtr->rdbuf()->pubsetbuf(fileBufferPtr.get(), bufSize);
	if (fstreamPtr->fail()) { // Throw if the file fails to open
		std::cerr << "Could not open file: " << fileName << std::endl;
		return false;
	}
	_fileOpen = true;
	return true;
}

bool cDataFile::closeFile() {
	fstreamPtr->close();
	_isWriting = false;
	_fileOpen = false;
	return true;
}

std::fstream* cDataFile::deprecatedGetfstreamPtr() { 
	return fstreamPtr.get(); 
} // To make it easy to transition correl cache file to new format

// File load and save
bool cDataFileMonolithic::loadFile(const std::string & fileName) {
	if (!_openFileRead(fileName)) throw_line("Could not load file");
	_isWriting = false;
	_procFile();
	return closeFile();
}

bool cDataFileMonolithic::saveFile(const std::string & fileName) {
	if (!_newFileWrite(fileName)) throw_line("Could not create file");
	_isWriting = true;
	_procFile();
	return closeFile();
}

// Process file
void cDataFileMonolithic::_procFile() {
	throw_line_overridden;
}

void cDataFile::procCStr(char * var, const std::size_t & fileCStrSize) {

	// Enforces null-termination of C-strings being saved
	if (var[fileCStrSize - 1]) {
		var[fileCStrSize - 1] = 0;
		std::string tmpErr = "Tried to write a non-null-terminated C-str: " + fileCStrSize;
		throw_line(tmpErr);
	}
	_procBytes(var, fileCStrSize);
};

// Handles writing of std strings to file and vice versa
void cDataFile::_procStrFLen(std::string & var, const std::size_t & fileCStrSize) {

	if (!fileCStrSize) throw_line("Cannot output C-str of 0 length!");
	
	const std::size_t convertedStrSize = var.size() + 1;
	std::size_t lengthToCopy = 0;

	if (_isWriting) {

		// If string is over max length, let user know that it is being truncated
		if (convertedStrSize > fileCStrSize) {
			std::cerr << "Truncating std::string for file output:\n\tOriginal:" << var << "\n";
			lengthToCopy = fileCStrSize - 1;
		}
		else {
			lengthToCopy = convertedStrSize - 1;
		}
		
		// Get a C-str pointer
		char * tmpCStr = &var[0];// new char[fileCStrSize];

		// Process the c string string
		TRY

		procCStr(tmpCStr, fileCStrSize);

		CATCH_RETHROW("Failed to output C-string to file")

	}
	else {

		// Make sure the var string is of sufficient length
		var.resize(fileCStrSize - 1);

		// Get a C-str pointer
		char * tmpCStr = &var[0];// new char[fileCStrSize];

		// Fill it with data
		try {
			procCStr(tmpCStr, fileCStrSize);
		}
		catch (...) {
			RETHROW("Failed to load C-string from file");
		}

		// Check if the returned string is null terminated; if not, null-terminate it and warn the user
		if (tmpCStr[fileCStrSize - 1]) {
			tmpCStr[fileCStrSize - 1] = 0;
			std::cerr << "Loaded string was not null-terminated; null-terminating string: \"" << fileCStrSize << "\"\n";
		}
	}
}

/// Writes variable-length strings; saves space at the cost of save file resilience
void cDataFile::procStr(std::string & var) {

	if (_isWriting) {

		//if (var == "</queries>") {
		//	std::cerr << "Break!";
		//}

		// Get the length of the string
		uint16_t cStrLen = static_cast<uint16_t>(var.size() + 1);

		// Makes sure it is not of a craaaaazy length
		const unsigned int maxLen = 10000;
		if (cStrLen > maxLen) {
			std::cerr << "Warning: length of string being output to file was greater than " << maxLen << "\n";
		}

		// Output length variable
		procVar(cStrLen);

		// Output cstr of required length
		_procStrFLen(var, cStrLen);
	}
	else {

		//if (var == "</queries>") {
		//	std::cerr << "Break!";
		//}

		uint16_t cStrLen = 0;

		// Get the length of the string
		procVar(cStrLen);

		// Makes sure it is not of a craaaaazy length
		const unsigned int maxLen = 10000;
		if (cStrLen > maxLen) {
			std::cerr << "Warning: length of string being read from file was greater than " << maxLen << "\n";
		}

		// Get cstr
		_procStrFLen(var, cStrLen);
	}
}

