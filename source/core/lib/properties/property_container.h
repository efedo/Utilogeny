#pragma once
#include <string>
#include <unordered_map>
#include <iostream>
#include <variant>
#include <any>
#include <cassert>
#include <iomanip>
#include <type_traits>
#include "Utilogeny/source/core/properties/property.h"

class PropertyContainer {
public:

	Property& property(const std::string& name) {
		registerIfNeeeded();
		// check built-ins
		if (builtin_properties.count(name)) {
			uint64_t offset = builtin_properties.at(name);
			char* mainclassptr = (char*)(this);
			return *((Property*)(mainclassptr + offset));
			// check addons
		}
		else if (addon_properties.count(name)) {
			return addon_properties.at(name);
		}
		else {
			std::cerr << "Cannot get property " << name << " from object, property does not exist.\n";
			return dummyBuiltin;
		}
	}

	void add_property(const std::string& name) {
	}

	//template <class tpPropertyType>
	//void add_property(const std::string& name) {
	//	registerIfNeeeded();
	//	if (addon_properties.count(name)) {
	//		std::cerr << "Cannot add property " << name << " to object, property already exists.\n";
	//		return;
	//	}
	//	addon_properties.emplace(name, Property(tpPropertyType()));
	//}

	template <class tpPropertyType>
	void add_property(const std::string& name, tpPropertyType val) {
		registerIfNeeeded();
		if (addon_properties.count(name)) {
			std::cerr << "Cannot add property " << name << " to object, property already exists.\n";
			return;
		}
		addon_properties.emplace(name, Property(val));
	}

	void remove_property(const std::string& name) {
		registerIfNeeeded();
		if (!addon_properties.count(name)) {
			std::cerr << "Cannot remove property " << name << " from object, property does not exist.\n";
			return;
		}
		addon_properties.erase(name);
	}

protected:
	void registerBuiltinProperty(const std::string& name, Property& propertyinst) {
		char* propertyptr = (char*)(&(propertyinst));
		char* mainclassptr = (char*)(this);
		uint64_t offset = propertyptr - mainclassptr;
		builtin_properties.emplace(name, offset);
	}
	virtual void registerBuiltinProperties() {}; // does nothing unless overidden

	void registerIfNeeeded() {
		// Registers built-ins
		if (!registeredBuiltinProperties) {
			registerBuiltinProperties();
			registeredBuiltinProperties = true;
		}
	}
private:
	friend class Property;
	bool registeredBuiltinProperties = false;
	std::unordered_map<std::string, Property> addon_properties;

	// Need a new one of these at each level!!!!!
	std::unordered_map<std::string, uint64_t> builtin_properties; // property memory offset relative to class pointer base

	static Property dummyBuiltin; // dummy return property
};
