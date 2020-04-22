// Copyright 2017-20 Eric Fedosejevs
//

#include "Utilogeny/source/precomp.h"
#include "Utilogeny/source/datafile.h"
#include "Utilogeny/source/exceptions.h"

// Section labels

void cDataFile::procLabel(const std::string & labelString) {

	// Make a modifiable string
	std::string tmpLabelString = labelString;

	// Print or get string
	procStrVLen(tmpLabelString);

	// Check if it matched
	if (!_isSaving) {

		// Check if equivalent
		if (tmpLabelString != labelString) {
			std::string errStr = "File label (\"" + labelString + "\") not in correct position. Instead read: \"";
			errStr = errStr + tmpLabelString + "\"";
			throw_line(errStr);
		}
	}

}

void cDataFile::_procVar(char * varPtr, const std::size_t & size) {
	if (_isSaving) {
		_printVar(varPtr, size);
	}
	else {
		_getVar(varPtr, size);
	}
}


void cDataFile::_getVar(char * varPtr, const std::size_t & size) {
	fstreamPtr->read(varPtr, size);
}

// Print variable
void cDataFile::_printVar(const char * varPtr, const std::size_t & size) {
	fstreamPtr->write(varPtr, size);
}

// File load and save
bool cDataFile::loadFile(const std::string & fileName) {
	_isSaving = false;
	const unsigned long bufSize = 10485760; // Set file buffer size to 10 mb
	std::unique_ptr<char []> fileBufferPtr(new char[bufSize]);
	fstreamPtr = new std::fstream(fileName, std::ios::binary | std::ios::in); // Create an input file stream
	fstreamPtr->rdbuf()->pubsetbuf(fileBufferPtr.get(), bufSize);
	if (fstreamPtr->fail()) { // Throw if the file fails to open
		std::cerr << "Could not open file: " << fileName << std::endl;
		return false;
	}
	_procFile();
	fstreamPtr->close();
	delete fstreamPtr;
	_isSaving = false;
	return true;
}

bool cDataFile::saveFile(const std::string & fileName) {
	_isSaving = true;
	const unsigned long bufSize = 10485760; // Set file buffer size to 10 mb
	std::unique_ptr<char[]> fileBufferPtr(new char[bufSize]);
	fstreamPtr = new std::fstream(fileName, std::ios::binary | std::ios::out | std::ios::trunc); // Create an input file stream
	fstreamPtr->rdbuf()->pubsetbuf(fileBufferPtr.get(), bufSize);
	if (fstreamPtr->fail()) { // Throw if the file fails to open
		std::cerr << "Could not open file: " << fileName << std::endl;
		return false;
	}
	_procFile();
	fstreamPtr->close();
	delete fstreamPtr;
	_isSaving = false;
	return true;
}

// Process file
void cDataFile::_procFile() {
	throw_line_overridden;
}

void cDataFile::procCStr(char * var, const std::size_t & fileCStrSize) {

	// Enforces null-termination of C-strings being saved
	if (var[fileCStrSize - 1]) {
		var[fileCStrSize - 1] = 0;
		std::string tmpErr = "Tried to write a non-null-terminated C-str: " + fileCStrSize;
		throw_line(tmpErr);
	}
	_procVar(var, fileCStrSize);
};

// Handles writing of std strings to file and vice versa
void cDataFile::procStrFLen(std::string & var, const std::size_t & fileCStrSize) {

	if (!fileCStrSize) throw_line("Cannot output C-str of 0 length!");
	
	const std::size_t convertedStrSize = var.size() + 1;
	std::size_t lengthToCopy = 0;

	if (_isSaving) {

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
		TRY_CODE

		procCStr(tmpCStr, fileCStrSize);

		CODE_FAILED_SILENT_RETHROW("Failed to output C-string to file")

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
			RETHROW_LINE("Failed to load C-string from file");
		}

		// Check if the returned string is null terminated; if not, null-terminate it and warn the user
		if (tmpCStr[fileCStrSize - 1]) {
			tmpCStr[fileCStrSize - 1] = 0;
			std::cerr << "Loaded string was not null-terminated; null-terminating string: \"" << fileCStrSize << "\"\n";
		}
	}
}

/// Writes variable-length strings; saves space at the cost of save file resilience
void cDataFile::procStrVLen(std::string & var) {

	if (_isSaving) {

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
		procStrFLen(var, cStrLen);
	}
	else {

		uint16_t cStrLen;

		// Get the length of the string
		procVar(cStrLen);

		// Makes sure it is not of a craaaaazy length
		const unsigned int maxLen = 10000;
		if (cStrLen > maxLen) {
			std::cerr << "Warning: length of string being read from file was greater than " << maxLen << "\n";
		}

		// Get cstr
		procStrFLen(var, cStrLen);
	}
}

