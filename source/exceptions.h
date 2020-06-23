// Copyright 2017-20 Eric Fedosejevs
//

#pragma once
#include "Utilogeny/source/precomp.h"
#include "Utilogeny/source/Utilogeny.h"

class cException;

void _print_exception_func(const cException&, unsigned int level = 0);

//class cException : public std::exception {
class cException {
public:

	// Creates copies of exceptions
	cException(const cException * const);
	cException(const cException&);

	// Create new exceptions without nested
	cException();
	cException(const std::exception&);

	// Create new exceptions with nested

	cException(const std::string &, const std::string &, int, std::thread::id, const cException * const = 0);
	cException(const char * const, const char * const, int, std::thread::id, const cException * const = 0);
	cException(const std::string &, const std::string &, int, std::thread::id, const cException &);
	cException(const char * const, const char * const, int, std::thread::id, const cException &);

	cException(const std::string &, const cException * const = 0);
	cException(const std::string &, const cException &);

	cException(const char* const, const cException * const = 0);
	cException(const char* const, const cException &);

	cException(const std::string &, const std::exception &);
	cException(const char * const, const std::exception &);

	// Destructor
	~cException(); // needs to delete all nested exceptions

	// Member access functions
	const std::string getDescription() const { return description; };
	const std::string getFile() const { return std::string(file); };
	const std::string getLine() const { return std::to_string(line); };
	const std::thread::id getThreadID() const { return threadID; };
	static void setConsoleOutMutex(std::recursive_mutex* tmpMtx) { consoleOutMutex = tmpMtx; };
	void print() { _print_exception_func(*this); }
private:
	cException* nestedExceptionPtr = 0;
	friend void _print_exception_func(const cException &, unsigned int);
	std::string description = "";
	std::string file = "";
	int line = 0;
	std::thread::id threadID;
	static std::recursive_mutex* consoleOutMutex;
};

#define throw_line(arg)					throw cException(arg, __FILE__, __LINE__, std::this_thread::get_id());
#define throw_line_overridden			throw_line("Virtual member should have been overridden")
#define throw_line_not_implemented		throw_line("Not yet implemented")
#define throw_line_deprecated			throw_line("Deprecated code")

#define _NEST(arg, exc)					cException(arg, __FILE__, __LINE__, std::this_thread::get_id(), &exc)
#define _NEST_RETHROW(arg, exc)			throw _NEST(arg, exc);
#define _PRINT_EXCEPTION(exc)			std::lock_guard<std::recursive_mutex> lock(Utilogeny::consoleOutMutex); std::cerr << "\nException caught by " << __FILE__ << ":" << __LINE__ << " (thread " << std::this_thread::get_id() << ")\n\n";	_print_exception_func(exc); std::cerr << "\n";

#define TRY								try{
#define RETHROW(arg)					try { throw; } catch (const std::exception& e) { _NEST_RETHROW(arg, cException(e)) } catch (const cException& e) { _NEST_RETHROW(arg, e) } catch (...) { _NEST_RETHROW(arg, cException()) }
#define CATCH_VOCAL_RETHROW(arg)		} catch (...) { try { RETHROW(arg) } catch (const cException& e) { _PRINT_EXCEPTION(e); throw e; } catch (...) { } }
#define CATCH_RETHROW(arg)				} catch (...) { try { RETHROW(arg) } catch (const cException& e) {                      throw e; } catch (...) { } }
#define CATCH_VOCAL(arg)				} catch (...) { try { RETHROW(arg) } catch (const cException& e) { _PRINT_EXCEPTION(e);		     } catch (...) { } }

class cExceptionRemoveBadRuns {
public:
	std::vector<std::string> badRunNames;
};