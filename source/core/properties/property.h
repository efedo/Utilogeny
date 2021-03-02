#pragma once
#include <string>
#include <unordered_map>
#include <iostream>
#include <variant>
#include <any>
#include <cassert>

// Synopsis
//
// Built-in properties
// (higher performance)
//
// dog.bark.freq = 5;
// dog.property("bark").property("freq") = 5;
// int blah1 = dog.property("bark").property("freq");
// string blah2 = dog.property("bark").property("freq"); // ERROR not string compatible
// int blah3 = dog.bark.freq; 
// dog.remove_property("bark_freq") ERROR cannot remove built-in property

// Add-on properties
//
// dog = animal;
// dog.add_property<PropertyGroup>("bark");
// dog.property("bark").add_property("freq"); // nested property
// int blah = dog.property("bark").property("freq");
// string blah = dog.property("bark").property("freq"); // ERROR not string compatible
// dog.remove_property("bark") OKAY removed from this dog
// cat = dog; // clone dog, including properties

class Property;

class PropertyContainer {
public:

	Property& property(const std::string& name) {
		registerIfNeeeded();
		// check built-ins
		if (builtin_properties.count(name)) {
			int offset = builtin_properties.at(name);
			char* mainclassptr = (char*)(this);
			return *((Property*)(mainclassptr + offset));
		// check addons
		} else if(addon_properties.count(name)) {
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
	void registerBuiltinProperty(const std::string& name, Property & propertyinst) {
		char* propertyptr = (char*)(&(propertyinst));
		char* mainclassptr = (char*)(this);
		int offset = propertyptr - mainclassptr;
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
	std::unordered_map<std::string, size_t> builtin_properties; // property memory offset relative to class pointer base
	static Property dummyBuiltin; // dummy return property
};

class Property {
public:

	Property() {}

	template <class tpPropertyType>
	Property(tpPropertyType init) : val(init) {}

	Property & operator=(const Property & rhs)
	{
		assert(val.index() == rhs.val.index()); // Check that both properties have same index
		val = rhs.val;
		return *this;
	}

	template <class tpPropertyType>
	operator tpPropertyType &() {
		return this->get<tpPropertyType>();
	}

	template <class tpPropertyType>
	tpPropertyType & get() {
		return std::get<tpPropertyType>(val); //static_cast<tpPropertyType>(
	}

	template <class tpPropertyType>
	void set(tpPropertyType newval) {
		size_t currindex = val.index;
		// Except on first init, don't worry about type
		static_assert(std::is_convertible<tpPropertyType, std::variant_alternative_t<val.index, val >>::value);
		static_cast<std::variant_alternative_t<val.index, val >>(newval); // try to cast to existing type and see what happens
		//static_assert(std::is_same_v<tpPropertyType, std::variant_alternative_t<val.index, val>>);
	}
private:
	std::variant<bool, int, float, double, std::string> val;
};

