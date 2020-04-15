// Copyright 2017-18 The Curators of the University of Missouri
//

#include "Utilogeny/source/precomp.h"
#include "Utilogeny/source/exceptions.h"
//#include "source/core/globals.h"

std::recursive_mutex* cException::consoleOutMutex = 0;

cException::cException(const std::string & tmpDesc, const char *tmpFile, int tmpLine, std::thread::id tmpID, cException* tmpNestedPtr)
	: description(tmpDesc), file(tmpFile), line(tmpLine), threadID(tmpID) 
{
	if (tmpNestedPtr) {
		nestedExceptionPtr = new cException(tmpNestedPtr);
	}
}

cException::cException(const std::string& tmpDesc, const char* tmpFile, int tmpLine, std::thread::id tmpID, const cException& tmpNestedPtr)
	: cException(tmpDesc, tmpFile, tmpLine, tmpID, (cException *)(&tmpNestedPtr))
{}

cException::cException(const cException* exceptionPtr)
	: description(exceptionPtr->description), file(exceptionPtr->file), line(exceptionPtr->line), threadID(exceptionPtr->threadID)
{
	if (exceptionPtr->nestedExceptionPtr) {
		nestedExceptionPtr = new cException(exceptionPtr->nestedExceptionPtr);
	}
}

// needs to create a new exception object with nested exception based on std::exception
cException::cException(const std::exception & e)
	: description(e.what()), file(""), line(0), threadID(), nestedExceptionPtr(0)
{}

// needs to create a new exception object with nested exception based on std::exception
cException::cException()
	: description("Unknown exception"), file(""), line(0), threadID(), nestedExceptionPtr(0)
{}

// needs to delete all nested exceptions
cException::~cException() {
	if (nestedExceptionPtr) {
		delete nestedExceptionPtr;
		nestedExceptionPtr = 0;
	}
}

// Recursively describes nested exceptions
void print_exception(const cException & internalE, unsigned int level)
{
	std::unique_ptr<std::lock_guard<std::recursive_mutex>> lockGuardPtr;
	if (cException::consoleOutMutex) {
		lockGuardPtr.reset(new std::lock_guard<std::recursive_mutex>(*(cException::consoleOutMutex)));
	}

	// Head level
	if (!level) {
		// If it is at the head level, print exception trace
		std::cerr << "Exception caught in thread " << std::this_thread::get_id() << ". Trace:\n";
		// Get a console output lock (necessary for multi-threaded cases)
	}

	// Indent according to exception level
	std::cerr << "\t" << std::string(level, ' ');

	// If it's an RNA-see exception, provide extra information
	//const cException * internalE = dynamic_cast<const cException *>(&e);
	//if (internalE) {
		std::cerr << "RNA-see @ " << internalE.getFile() << ":" << internalE.getLine();
		std::cerr << " (thread " << internalE.getThreadID() << ") - \"";
		std::cerr << internalE.getDescription() << "\"\n";
	//}
	//else {
	//	std::cerr << "STL - \"";
	//	std::cerr << e.what() << "\"\n";
	//}

	// See if it's a nested exception
	if (internalE.nestedExceptionPtr) {
		print_exception(internalE.nestedExceptionPtr, level + 1);
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