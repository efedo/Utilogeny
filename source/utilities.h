// Copyright 2017-18 The Curators of the University of Missouri
//

#pragma once
#include "Utilogeny/source/precomp.h"
//#include "source/core/typedefs.h"

// String check commands; mostly for regex verification purposes
bool checkFixRegexString(std::string &);
bool isAllAlphanum(const std::string &);
bool isAllNum(const std::string &);

// String utility commands
void replaceUnderscoresAndTrimSpaces(std::string &);
void toUpper(std::string &); // Converts string to upper case
void toLower(std::string &); // Converts string to upper case

// String/char to number conversions
float strToFloat(const std::string &);
double strToDoub(const std::string &);
float _strToFloatSafe(const std::string&);
double _strToDoubSafe(const std::string &);
unsigned long strToULongFast(const std::string &);
bool strIsInt(const std::string &);
bool strToBool(const std::string &);
unsigned int charToInt(char &);

// Number to string conversions
std::string doubleToString(const double &, const unsigned int &);

// Line split commands
void splitLineOnTabs(const std::string &, std::vector<std::string> &, const size_t & = 10);
inline void splitLineOnChar(const std::string &, const char &, std::vector<std::string> &, const size_t & = 10);
std::vector<std::string> splitCommandLineFast(const std::string &);

// File utilities
bool fileExists(const std::string &);
void ensureSafeFilenameOverwrite(std::string &);
void checkOutputFilename(std::string &);


//// Thread utilities
//// Note small memory leak due to lack of reference counting (see destructor)
//class cSmartThreadPtr {
//public:
//	void setComplete(); // Indicates that the thread is complete and is ready to join
//	bool isComplete();
//	bool isAssigned();
//	bool joinIfComplete(); // Joins the calling thread if execution is complete
//	cSmartThreadPtr();
//	void assignThread(std::thread *);
//	~cSmartThreadPtr();
//private:
//	std::thread ** threadPtrPtr;
//	std::atomic_bool * threadCompletePtr;
//};

// Misc.

typedef std::vector<std::pair<unsigned long, unsigned long> > binBoundaryPairVector;

/// Splits a group into the desired number of bins and returns the bin boundaries
std::vector<std::pair<unsigned long, unsigned long>> splitGroupIntoBins(const unsigned long, const unsigned int);


//// To get underlying enum type
//template <typename E>
//constexpr typename std::underlying_type<E>::type & to_underlying(E & e) noexcept {
//	//return static_cast<typename std::underlying_type<E>::type &>(e);
//	return *((std::underlying_type<E>::type *)(&e));
//}

// try moving to 