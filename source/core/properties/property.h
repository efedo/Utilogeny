#pragma once
#include <string>
#include <unordered_map>
#include <iostream>
#include <variant>
#include <any>
#include <cassert>
#include <iomanip>
#include <type_traits>

// Adds 9 bytes + string (created only as needed)
class Property {
public:

	enum class PropertyStatusEnum : uint8_t {
		t_uninitialized = 0,
		t_stack = 1,
		t_heap = 2
	};

	enum class PropertyTypeEnum : uint8_t {
		t_invalid = 0,
		t_bool = 1,
		t_int = 2,
		t_float = 3,
		t_double = 4,
		t_string = 5
	};

	~Property() {
		if ((status == PropertyStatusEnum::t_heap) && location) delete location;
		location = 0;
		status = PropertyStatusEnum::t_uninitialized;
		ptype = PropertyTypeEnum::t_invalid;
	}

	Property& operator=(Property& rhs) {
		if (&rhs == this) return *this;
		
		if ((status == PropertyStatusEnum::t_stack) ||
			(status == PropertyStatusEnum::t_heap)) {
			if ((rhs.status == PropertyStatusEnum::t_stack) ||
				(rhs.status == PropertyStatusEnum::t_heap))
			{
				switch (rhs.ptype) {
				case PropertyTypeEnum::t_bool:
					this->setVal<bool>(rhs.getVal<bool>());
				case PropertyTypeEnum::t_int:
					this->setVal<int>(rhs.getVal<int>());
				case PropertyTypeEnum::t_float:
					this->setVal<float>(rhs.getVal<float>());
				case PropertyTypeEnum::t_double:
					this->setVal<double>(rhs.getVal<double>());
				case PropertyTypeEnum::t_string:
					this->setVal<std::string>(rhs.getVal<std::string>());
				default:
					std::cerr << "Assigned value not convertible to existing property value!\n";
				}
			} else {
				std::cerr << "Property error: cannot assign from uninitialized property\n";
			}
		} else {
			// this property is unitialized
			if (rhs.status == PropertyStatusEnum::t_static)
			{
				// copy other property's contents
				status = rhs.status;
				location = rhs.location;
				ptype = rhs.ptype;
			}
			else if (rhs.status == PropertyStatusEnum::t_dynamic)
			{
				// steal other property's contents
				status = rhs.status;
				rhs.status = PropertyStatusEnum::t_uninitialized;
				location = rhs.location;
				rhs.location = 0;
				ptype = rhs.ptype;
				rhs.ptype = Property::PropertyTypeEnum::t_invalid;
			}
			else {
				std::cerr << "Property error: cannot assign from uninitialized property\n";
			}
		}
		return *this;
	}

	Property(const Property& rhs) {
		if (rhs.status == PropertyStatusEnum::t_static)
		{
			// copy other property's contents
			status = rhs.status;
			location = rhs.location;
			ptype = rhs.ptype;
		}
		else if (rhs.status == PropertyStatusEnum::t_dynamic)
		{
			// steal other property's contents
			status = rhs.status;
			rhs.status = PropertyStatusEnum::t_uninitialized;
			location = rhs.location;
			rhs.location = 0;
			ptype = rhs.ptype;
			rhs.ptype = Property::PropertyTypeEnum::t_invalid;
		}
		else {
			std::cerr << "Property error: cannot assign from uninitialized property\n";
		}
	}

	template <class tpPropertyType>
	constexpr PropertyTypeEnum getTypeEnum() const {
		if constexpr (std::is_same_v<tpPropertyType, bool>) {
			return PropertyTypeEnum::t_bool;
		}
		else if constexpr (std::is_same_v<tpPropertyType, int>) {
			return PropertyTypeEnum::t_int;
		}
		else if constexpr (std::is_same_v<tpPropertyType, float>) {
			return PropertyTypeEnum::t_float;
		}
		else if constexpr (std::is_same_v<tpPropertyType, double>) {
			return PropertyTypeEnum::t_double;
		}
		else if constexpr (std::is_same_v<tpPropertyType, std::string>) {
			return PropertyTypeEnum::t_string;
		}
		else {
			return PropertyTypeEnum::t_invalid;
		}
	}

	template <class tpPropertyType>
	tpPropertyType getVal() const {
		switch (ptype) {
		case PropertyTypeEnum::t_bool:
			if constexpr (std::is_convertible_v<bool, tpPropertyType>) return *((bool*)(location));
		case PropertyTypeEnum::t_int:
			if constexpr (std::is_convertible_v<int, tpPropertyType>) return *((int*)(location));
		case PropertyTypeEnum::t_float:
			if constexpr (std::is_convertible_v<float, tpPropertyType>) return *((float*)(location));
		case PropertyTypeEnum::t_double:
			if constexpr (std::is_convertible_v<double, tpPropertyType>) return *((double*)(location));
		case PropertyTypeEnum::t_string:
			if constexpr (std::is_convertible_v<std::string, tpPropertyType>) return *((std::string*)(location));
		}
		std::cerr << "Property value not convertible to desired type!\n";
	}

	template <class tpPropertyType>
	void setVal(const tpPropertyType & newval) {
		switch (ptype) {
		case PropertyTypeEnum::t_bool:
			if constexpr (std::is_convertible_v<bool, tpPropertyType>) *(bool*)(location) = newval;
		case PropertyTypeEnum::t_int:
			if constexpr (std::is_convertible_v<int, tpPropertyType>) *(int*)(location) = newval;
		case PropertyTypeEnum::t_float:
			if constexpr (std::is_convertible_v<float, tpPropertyType>) *(float*)(location) = newval;
		case PropertyTypeEnum::t_double:
			if constexpr (std::is_convertible_v<double, tpPropertyType>) *(double*)(location) = newval;
		case PropertyTypeEnum::t_string:
			if constexpr (std::is_convertible_v<std::string, tpPropertyType>) *(std::string*)(location) = newval;
		default:
			std::cerr << "Assigned value not convertible to existing property value!\n";
		}
	}

	template <class tpPropertyType>
	tpPropertyType& getRef() const {
		switch (ptype) {
		case PropertyTypeEnum::t_bool:
			return *((bool*)(location));
		case PropertyTypeEnum::t_int:
			return *((int*)(location));
		case PropertyTypeEnum::t_float:
			return *((float*)(location));
		case PropertyTypeEnum::t_double:
			return *((double*)(location));
		case PropertyTypeEnum::t_string:
			return *((std::string*)(location));
		}
		std::cerr << "Property value not convertible to desired type!\n";
	}

	std::string getTypeString() const {
		switch (ptype) {
		case PropertyTypeEnum::t_invalid:
			return "invalid";
		case PropertyTypeEnum::t_bool:
			return "bool";
		case PropertyTypeEnum::t_int:
			return "int";
		case PropertyTypeEnum::t_float:
			return "float";
		case PropertyTypeEnum::t_double:
			return "double";
		case PropertyTypeEnum::t_string:
			return "string";
		}
		return "unknown type";
	}

	std::string getValString() const {
		std::stringstream ss;

		switch (ptype) {
		case PropertyTypeEnum::t_bool:
			ss << *((bool*)(location));
		case PropertyTypeEnum::t_int:
			ss << *((int*)(location));
		case PropertyTypeEnum::t_float:
			ss << *((float*)(location));
		case PropertyTypeEnum::t_double:
			ss << *((double*)(location));
		case PropertyTypeEnum::t_string:
			ss << *((std::string*)(location));
		default:
			std::cerr << "Property value not convertible to desired type!\n";
			return "unknown";
		}
		return ss.str();
	}

	explicit operator bool() { return getVal<bool>(); }
	explicit operator int() { return getVal<int>(); }

	template <class tpPropertyType>
	friend bool operator==(const Property& lhs, const tpPropertyType& rhs) {
		return (lhs.getVal<tpPropertyType>() == rhs);
	}

	template <class tpPropertyType>
	friend bool operator==(const tpPropertyType& lhs, const Property& rhs) {
		return (rhs == lhs);
	}

	//template <class tpPropertyType>
	//friend bool operator==(const Property& lhs, const Property& rhs) {

	//}

private:
	friend class DynamicProperty;

	template <class tpPropertyType>
	friend Property registerStackProperty(tpPropertyType&);

	template <class tpPropertyType>
	friend Property getDynamicProperty();

	template <class tpPropertyType>
	friend Property getDynamicProperty(const tpPropertyType&);

	PropertyTypeEnum ptype = PropertyTypeEnum::t_invalid; // 1 byte
	void* location = 0; // 8 bytes
	PropertyStatusEnum status = PropertyStatusEnum::t_uninitialized;

	template <class tpPropertyType>
	Property(tpPropertyType* propLoc) : ptype(getTypeEnum<tpPropertyType>()), location((void *)(propLoc)) {}
	Property() {}

	template <class tpPropertyType>
	void init(tpPropertyType* propLoc) {
		ptype = getTypeEnum<tpPropertyType>();
		location = ((void*)(propLoc));
	}
};

template <class tpPropertyType>
Property registerStackProperty(tpPropertyType& property) {
	return Property(&property);
}

template <class tpPropertyType>
Property getDynamicProperty() {
	Property dynprop;
	dynprop.init<tpPropertyType>(new tpPropertyType);
	return std::move(dynprop);
}

template <class tpPropertyType>
Property getDynamicProperty(const tpPropertyType & val) {
	Property dynprop;
	dynprop.init<tpPropertyType>(new tpPropertyType(val));
	//return std::move(dynprop);
	return dynprop;
}
