// Copyright 2017-20 Eric Fedosejevs
//

#pragma once
#include "Utilogeny/source/precomp.h"

template <class T>
class cFastArray;
template <class ParamPack>
class cSparseArray;

class cDataFile {
public:
	bool openFile(const std::string&, std::ios::openmode);
	bool closeFile();
	std::fstream* deprecatedGetfstreamPtr(); // To make it easy to transition correl cache file to new format
	bool _fileOpen = false; // dep; make accessor

	template <class T>
	friend class cFastArray;
	template <class ParamPack>
	friend class cSparseArray;

	// Write variable on save, check agreement on load
	template <typename T>
	void checkVar(const T& var, const std::string & err = "") {
		static_assert(std::negation<std::is_array<T>>::value, "Arrays not supported");
		static_assert(std::is_fundamental<T>::value, "Only fundamental types supported");
		T read_var = var;
		procVar(read_var);
		if (read_var != read_var) {
			std::string errStr = err + "\n" + "Expected: \"" + std::to_string(var) + "\". Instead read: \"" + std::to_string(read_var) + "\"";
			throw_line(errStr);
		}
	}
	void checkStr(const std::string &, const std::string & = "");

	// Process variable
	template <typename T>
	void procVar(T& var) {
		static_assert(std::negation<std::is_array<T>>::value, "Arrays not supported");
		static_assert(std::is_fundamental<T>::value, "Only fundamental types supported");
		_procBytes((char*)&var,  sizeof(var));
	};
	void procStr(std::string&); /// Writes variable-length strings; saves space at the cost of save file resilience
	void procCStr(char * var, const std::size_t & size);
	void procBytes(void * loc, size_t size) { _procBytes((char*)loc, size); };
	template <class T> void procArray(cFastArray<T>& array) { array.proc_file(*this); } // Processes fast arrays
	template <class ParamPack> void procArray(cSparseArray<ParamPack>& array) { array.proc_file(*this); } // Processes sparse arrays

	// Provided for dedicated write operations that require a const object
	void printVar(const char* var, const std::size_t& size) { _printBytes(var, size); };

	bool isSaving() { return _isWriting; };
	bool isLoading() { return !_isWriting; };

protected:
	void _procStrFLen(std::string&, const std::size_t&); // Writes fixed-length strings; more file resilience, but more bugs and larger file size

	// Process variable (handles both get and print)
	void _procBytes(char*, const std::size_t&);
	void _getBytes(char*, const std::size_t&);
	void _printBytes(const char*, const std::size_t&);

	// Whether saving or loading the file
	std::unique_ptr <std::fstream> fstreamPtr;
	std::unique_ptr<char[]> fileBufferPtr;

	bool _isWriting = false;
	bool _isOpen = false;
};

class cDataFileRandomAccess : public cDataFile {
public:
	bool newFileReadWrite(const std::string&);
	bool openFileReadWrite(const std::string&);
	void setRead() { _isWriting = true; }
	void setWrite() { _isWriting = false; }
	void seek(const size_t pos) {
		if (_isWriting) fstreamPtr->seekp(pos);
		else fstreamPtr->seekg(pos);
	}
};

class cDataFileMonolithic : public cDataFile {
public:
	bool loadFile(const std::string&);
	bool saveFile(const std::string&);
protected:
	bool _openFileRead(const std::string&);
	bool _newFileWrite(const std::string&);
	virtual void _procFile();
};