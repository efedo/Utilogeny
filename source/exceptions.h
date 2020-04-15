// Copyright 2017-18 The Curators of the University of Missouri
//

#pragma once
#include "Utilogeny/source/precomp.h"
#include "Utilogeny/source/Utilogeny.h"

//class cException : public std::exception {
class cException {
public:
	cException(const std::string&, const char* file, int line, std::thread::id, const cException & tmpNestedPtr);
	cException(const std::string &, const char *file, int line, std::thread::id, cException* tmpNestedPtr = 0);
	cException(const std::exception &); // needs to create a new exception object with nested exception based on std::exception
	cException(const cException*);
	cException();
	~cException(); // needs to delete all nested exceptions
	const std::string getDescription() const { return description; };
	const std::string getFile() const { return std::string(file); };
	const std::string getLine() const { return std::to_string(line); };
	const std::thread::id getThreadID() const { return threadID; };
	static void setConsoleOutMutex(std::recursive_mutex* tmpMtx) { consoleOutMutex = tmpMtx; };
private:
	cException* nestedExceptionPtr = 0;
	friend void print_exception(const cException &, unsigned int);
	const std::string description;
	const char *file;
	const int line;
	const std::thread::id threadID;
	static std::recursive_mutex* consoleOutMutex;
};

#define throw_line(arg) throw cException(arg, __FILE__, __LINE__, std::this_thread::get_id());
#define throw_line_overridden throw_line("Virtual member should have been overridden")
#define throw_line_not_implemented throw_line("Not yet implemented")
#define throw_line_deprecated throw_line("Deprecated code")
//#define rethrow_line(arg) std::throw_with_nested(cException(arg, __FILE__, __LINE__, std::this_thread::get_id()));

#define rethrow_line_packaged(arg, exc) throw cException(arg, __FILE__, __LINE__, std::this_thread::get_id(), exc);
#define rethrow_line(arg) try { throw; } catch (const std::exception & e) { rethrow_line_packaged(arg, cException(e)); } catch (const cException & e) { rethrow_line_packaged(arg, e); } catch (...) { rethrow_line_packaged(arg, cException()); }

void print_exception(const cException &, unsigned int level = 0);

// rethrow needs to include exception object

#define TRY_CODE try{
#define CODE_FAILED_VOCAL_RETHROW(arg) } catch (...) { std::lock_guard<std::recursive_mutex> lock(Utilogeny::consoleOutMutex); std::cerr << "Exception: " << arg; std::cerr << "Exception caught by " << __FILE__ << ":" << __LINE__ << "\n";	std::cerr << "\n"; rethrow_line(arg); }
#define CODE_FAILED_SILENT_RETHROW(arg) } catch (...) { rethrow_line(arg); }
#define CODE_FAILED_VOCAL_CATCH(arg) } catch (const std::exception & e) { try { std::lock_guard<std::recursive_mutex> lock(Utilogeny::consoleOutMutex); std::cerr << "Exception: " << arg; std::cerr << "Exception caught by " << __FILE__ << ":" << __LINE__ << "\n"; print_exception(cException(e)); std::cerr << "\n"; } catch (...) { /* extra catch to make no_except */ } } catch (const cException & e) { try { std::lock_guard<std::recursive_mutex> lock(Utilogeny::consoleOutMutex); std::cerr << "Exception: " << arg; std::cerr << "Exception caught by " << __FILE__ << ":" << __LINE__ << "\n"; print_exception(e); std::cerr << "\n"; } catch (...) { try { std::lock_guard<std::recursive_mutex> lock(Utilogeny::consoleOutMutex); std::cerr << "Exception: " << arg; std::cerr << "Exception caught by " << __FILE__ << ":" << __LINE__ << "\n"; std::cerr << "\n"; } catch (...) { /* extra catch to make no_except */ } } }

class cExceptionRemoveBadRuns {
public:
	std::vector<std::string> badRunNames;
};