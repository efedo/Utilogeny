// Copyright 2017-20 Eric Fedosejevs
//

#include "Utilogeny/source/core/precomp.h"
#include "Utilogeny/source/core/exceptions.h"
//#include "source/core/globals.h"

std::recursive_mutex* cException::consoleOutMutex = 0;

cException::cException(const cException* const exceptionPtr)
	: description(), file(exceptionPtr->file), line(exceptionPtr->line), threadID(exceptionPtr->threadID)
{
	if (exceptionPtr) {
		description = exceptionPtr->description;
		file = exceptionPtr->file;
		line = exceptionPtr->line;
		threadID = exceptionPtr->threadID;
		if (exceptionPtr->nestedExceptionPtr) {
			nestedExceptionPtr = new cException(exceptionPtr->nestedExceptionPtr);
		}
	}
}

cException::cException(const cException& exception)
	: cException(&exception)
{}

// needs to create a new exception object with nested exception based on std::exception
cException::cException()
	: description("Unknown exception")
{}

cException::cException(const std::string & tmpDesc, const std::string & tmpFile, int tmpLine, std::thread::id tmpID, const cException * const tmpNestedPtr)
	: description(tmpDesc), file(tmpFile), line(tmpLine), threadID(tmpID) 
{
	if (tmpNestedPtr) { nestedExceptionPtr = new cException(tmpNestedPtr); }
}

cException::cException(const std::string & tmpDesc, const std::string & tmpFile, int tmpLine, std::thread::id tmpID, const cException & tmpNestedPtr)
	: cException(tmpDesc, tmpFile, tmpLine, tmpID, (cException *)(&tmpNestedPtr))
{}

cException::cException(const std::string & tmpDesc, const cException * const tmpNestedPtr) 
: description(tmpDesc)
{
	if (tmpNestedPtr) { nestedExceptionPtr = new cException(tmpNestedPtr); }
}

cException::cException(const std::string & tmpDesc, const cException & tmpNestedPtr)
: cException(tmpDesc, (cException*)&tmpNestedPtr)
{}

// needs to create a new exception object with nested exception based on std::exception
cException::cException(const std::exception & e)
: description(e.what()), nestedExceptionPtr(0)
{}

cException::cException(const std::string & tmpStr, const std::exception& e)
: description(e.what()), nestedExceptionPtr(new cException(e))
{}


/////
// Const char variants, nothing special
////

cException::cException(const char* const tmpDesc, const char* const tmpFile, int tmpLine, std::thread::id tmpID, const cException* const tmpNestedPtr)
	: cException(std::string(tmpDesc), tmpFile, tmpLine, tmpID, tmpNestedPtr)
{}

cException::cException(const char* const tmpDesc, const char* const tmpFile, int tmpLine, std::thread::id tmpID, const cException& tmpNestedPtr)
	: cException(std::string(tmpDesc), tmpFile, tmpLine, tmpID, tmpNestedPtr)
{}

cException::cException(const char* const tmpDesc, const cException* const tmpNestedPtr)
	: cException(std::string(tmpDesc), tmpNestedPtr)
{}

cException::cException(const char* const tmpDesc, const cException& tmpNestedPtr)
	: cException(tmpDesc, (cException*)&tmpNestedPtr)
{}

cException::cException(const char* const tmpStrPtr, const std::exception& e)
	: cException(std::string(tmpStrPtr), e)
{}



// needs to delete all nested exceptions
cException::~cException() {
	if (nestedExceptionPtr) {
		//std::cout << "Delete nested pointer: " << nestedExceptionPtr << " (parent: " << this << ")\n";
		delete nestedExceptionPtr;
		nestedExceptionPtr = 0;
	}
}

// Recursively describes nested exceptions
void _print_exception_func(const cException & internalE, unsigned int level)
{
	std::unique_ptr<std::lock_guard<std::recursive_mutex>> lockGuardPtr;
	if (cException::consoleOutMutex) {
		lockGuardPtr.reset(new std::lock_guard<std::recursive_mutex>(*(cException::consoleOutMutex)));
	}

	//// Head level
	if (!level) {
	//	// If it is at the head level, print exception trace
		std::cerr << "Exception trace:\n";
	}

	// Indent according to exception level
	std::cerr << std::string(level + 1, ' ');

	// If it's an RNA-see exception, provide extra information
	//if (internalE) {
		//std::cerr << &internalE << " @ "; // Prints exception pointer (for exception handler troubleshooting)
		std::cerr << "Thread " << internalE.getThreadID() << ": ";
		std::cerr << internalE.getFile() << ":" << internalE.getLine();
		std::cerr << " - " << internalE.getDescription() << "\n";
	//}
	//else {
	//	std::cerr << "STL - \"";
	//	std::cerr << e.what() << "\"\n";
	//}

	// See if it's a nested exception
	if (internalE.nestedExceptionPtr) {
		_print_exception_func(internalE.nestedExceptionPtr, level + 1);
	}

	//try {
	//	std::rethrow_if_nested(e);
	//}
	//catch (const std::exception& e) {
	//	print_exception(e, level + 1);
	//}
	//// If it was a nested exception but you can't tell what type the inner exception is
	//catch (...) {
	//	// Indent according to exception level
	//	std::cerr << std::string(level + 1, ' ');
	//	std::cerr << "ERROR - unknown type\n";
	//}
}