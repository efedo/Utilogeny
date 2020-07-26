// Copyright 2017-20 Eric Fedosejevs
//

#pragma once
#include "Utilogeny/source/precomp.h"

// Note that this will be reverse of usual, highest values first
template <class K, class T>
class topmap : public std::map<L, T, std::greater<K>> {
public:
	topmap(size_t maxvalues);
	void emplace();
	void insert();
};

// Lowest values first
template <class K, class T>
class bottommap : public std::map<L, T, std::less<K>> {
public:
	bottommap(size_t maxvalues);
	void emplace();
	void insert();
};