// Copyright 2017-18 The Curators of the University of Missouri
//

#pragma once
#include "Utilogeny/source/precomp.h"

class cDataFile {
private:

	// Whether saving or loading the file
	bool _isSaving = false;

	// Process variable (handles both get and print)
	void _procVar(char *, const std::size_t &);
	void _getVar(char *, const std::size_t &);
	void _printVar(const char *, const std::size_t &);

public:

	// File load and save
	bool loadFile(const std::string &);
	bool saveFile(const std::string &);

	// Whether saving or loading the file
	bool isSaving() { return _isSaving; };
	bool isLoading() { return !_isSaving; };

protected:

	// Process file
	virtual void _procFile();

	/// Process section labels: prints label on save, checks on load
	void procLabel(const std::string &);

	// Process variable
	void procVar(bool & var) { _procVar((char *)&var, sizeof(var)); };
	void procVar(uint8_t & var) { _procVar((char *)&var, sizeof(var)); };
	void procVar(uint16_t & var) { _procVar((char *)&var, sizeof(var)); };
	void procVar(uint32_t & var) { _procVar((char *)&var, sizeof(var)); };
	void procVar(uint64_t & var) { _procVar((char *)&var, sizeof(var)); };
	void procVar(int8_t & var) { _procVar((char *)&var, sizeof(var)); };
	void procVar(int16_t & var) { _procVar((char *)&var, sizeof(var)); };
	void procVar(int32_t & var) { _procVar((char *)&var, sizeof(var)); };
	void procVar(int64_t & var) { _procVar((char *)&var, sizeof(var)); };
	void procVar(float & var) { _procVar((char *)&var, sizeof(var)); };
	void procVar(double & var) { _procVar((char *)&var, sizeof(var)); };
	void procVar(char & var) { _procVar((char *)&var, sizeof(var)); };
	void procCStr(char * var, const std::size_t & size);

	/// Writes fixed-length strings; more file resilience, but more bugs and larger file size
	void procStrFLen(std::string & var, const std::size_t & size); // Handles writing of std strings to file and vice versa
	
	/// Writes variable-length strings; saves space at the cost of save file resilience
	void procStrVLen(std::string & var);

	// Provided for dedicated write operations that require a const object
	void printVar(const char * var, const std::size_t & size) { _printVar(var, size); };

	std::fstream * fstreamPtr;
};