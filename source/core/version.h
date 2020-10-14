// Copyright 2017-20 Eric Fedosejevs
//

#pragma once
#include "Utilogeny/source/precomp.h"

//class cBinaryFile;

class cVersion {
public:
	std::string getVersionString() const { 
		return (std::to_string(_majorVersion) + '.' + std::to_string(_minorVersion) + '.' + std::to_string(_revision));
	};

	cVersion(const uint16_t & tmpMajor, const uint16_t & tmpMinor, const char & tmpRev)
		: _majorVersion(tmpMajor), _minorVersion(tmpMinor), _revision(tmpRev) {}

	friend bool operator== (const cVersion& lhs, const cVersion& rhs)
	{
		if (lhs._majorVersion != rhs._majorVersion) return false;
		if (lhs._minorVersion != rhs._minorVersion) return false;
		if (lhs._revision != rhs._revision) return false;
		return true;
	}

	friend bool operator!= (const cVersion& lhs, const cVersion& rhs)
	{
		return !(lhs == rhs);
	}

	friend bool operator> (const cVersion& lhs, const cVersion& rhs)
	{
		if (lhs._majorVersion != rhs._majorVersion) return (lhs._majorVersion > rhs._majorVersion);
		if (lhs._minorVersion != rhs._minorVersion) return (lhs._minorVersion > rhs._minorVersion);
		if (lhs._revision != rhs._revision) return (lhs._revision > rhs._revision);
		return false;
	}

	friend bool operator>= (const cVersion& lhs, const cVersion& rhs)
	{
		if (lhs._majorVersion != rhs._majorVersion) return (lhs._majorVersion > rhs._majorVersion);
		if (lhs._minorVersion != rhs._minorVersion) return (lhs._minorVersion > rhs._minorVersion);
		if (lhs._revision != rhs._revision) return (lhs._revision > rhs._revision);
		return true;
	}

	friend bool operator< (const cVersion& lhs, const cVersion& rhs)
	{
		if (lhs._majorVersion != rhs._majorVersion) return (lhs._majorVersion < rhs._majorVersion);
		if (lhs._minorVersion != rhs._minorVersion) return (lhs._minorVersion < rhs._minorVersion);
		if (lhs._revision != rhs._revision) return (lhs._revision < rhs._revision);
		return false;
	}

	friend bool operator<= (const cVersion& lhs, const cVersion& rhs)
	{
		if (lhs._majorVersion != rhs._majorVersion) return (lhs._majorVersion < rhs._majorVersion);
		if (lhs._minorVersion != rhs._minorVersion) return (lhs._minorVersion < rhs._minorVersion);
		if (lhs._revision != rhs._revision) return (lhs._revision < rhs._revision);
		return true;
	}

private:
	friend class cBinaryFile;
	friend class cProjectFile;
	uint16_t _majorVersion;
	uint16_t _minorVersion;
	char _revision;
};