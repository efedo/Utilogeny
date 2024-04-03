// Previous variant implementation of property object
//
// Replaced because it has excessive overhead for stack properties since it 
// has to store a full variant for even simple types.
//
// Also extreme code bloat due to exponential template combinations with visitor pattern

#pragma once
#include <string>
#include <unordered_map>
#include <iostream>
#include <variant>
#include <any>
#include <cassert>
#include <iomanip>
#include <type_traits>


class PropertyOldVariant {
public:
	class InvalidType {};

	using prop_var_t = std::variant<InvalidType, bool, int, float, double, std::string, PropertyOldVariantContainer>; //AnyContainer

	template <class tpPropertyOldVariantType>
	friend PropertyOldVariant makePropertyOldVariant(const tpPropertyOldVariantType&);

	template <class tpPropertyOldVariantType>
	friend PropertyOldVariant makePropertyOldVariant();

	std::string getTypeString() {
		return _getVarTypeName(val);
	}

	std::string getValString() {
		return _getVarTypeVal(val);
	}

	PropertyOldVariant& operator=(const PropertyOldVariant& rhs)
	{
		if (val.index() == rhs.val.index()) {
			//if (val.index() == 0) {
			//	if (std::get<AnyContainer>(val).anydata.type() == std::get<AnyContainer>(rhs.val).anydata.type())
			//	{
			//		std::get<AnyContainer>(val) = std::get<AnyContainer>(rhs.val);
			//	}
			//	else {
			//		std::cerr << "Properties have different custom value types and cannot be copied\n";
			//	}
			//} else {
			val = rhs.val;
			//}
		}
		else {
			std::cerr << "PropertyOldVariant assignment error: PropertyOldVariant of type '" << _getVarTypeName(rhs.val) << "' cannot be copied to existing PropertyOldVariant of type '" << _getVarTypeName(val) << "'\n";
		}
		return *this;
	}

	template <class tpPropertyOldVariantType>
	PropertyOldVariant& operator=(const tpPropertyOldVariantType& rhs)
	{
		// https://en.cppreference.com/w/cpp/utility/variant/visit
		// Check that rhs is convertible to current type

		//std::cout << "Existing PropertyOldVariant is '" << _getVarTypeName(val) << "' with value '" << _getVarTypeVal(val) << "'\n";
		//std::cout << "Trying to set with '" << _getContainerizedTypeName<tpPropertyOldVariantType>() << "' of value '" << rhs << "'\n";

		std::visit([&](auto&& arg) {
			using T = std::decay_t<decltype(arg)>;
			// Override to prevent implicit conversion of floating point to integer types
			if constexpr (std::is_convertible_v<tpPropertyOldVariantType, T> && !(std::is_floating_point_v<tpPropertyOldVariantType> && std::is_integral_v<T>)) {
				val = (T)(rhs);
			}
			else {
				std::cerr << "PropertyOldVariant assignment error: Value of type '" << _getContainerizedTypeName<tpPropertyOldVariantType>() <<
					"' cannot be assigned to existing PropertyOldVariant of type '" << _getContainerizedTypeName<T>() << "'\n";
			}
			}, val);

		//std::cout << "PropertyOldVariant now set to '" << _getVarTypeName(val) << "' with value '" << _getVarTypeVal(val) << "'\n";
		return *this;
	}

	template <class tpPropertyOldVariantType>
	operator tpPropertyOldVariantType() const {
		return getVal<tpPropertyOldVariantType>();
	}

	template <class tpPropertyOldVariantType>
	tpPropertyOldVariantType getVal() const {
		tpPropertyOldVariantType returnval;
		std::visit([&](auto&& arg) {
			using T = std::decay_t<decltype(arg)>;
			// Override to prevent implicit conversion of floating point to integer types
			if constexpr (std::is_convertible_v<T, tpPropertyOldVariantType>) {
				returnval = (tpPropertyOldVariantType)(std::get<T>(val));
			}
			else {
				std::cerr << "PropertyOldVariant value not convertible to desired type!\n";
			}
			}, val);
		return returnval;
	}

	template <class tpPropertyOldVariantType>
	friend bool operator==(const PropertyOldVariant& lhs, const tpPropertyOldVariantType& rhs) {
		return (lhs.getVal<tpPropertyOldVariantType>() == rhs);
	}

	template <class tpPropertyOldVariantType>
	friend bool operator==(const tpPropertyOldVariantType& lhs, const PropertyOldVariant& rhs) {
		return (lhs == rhs.getVal<tpPropertyOldVariantType>());
	}

	friend bool operator==(const PropertyOldVariant& lhs, const PropertyOldVariant& rhs) {
		bool returnval;
		std::visit([&](auto&& argl) {
			using Tl = std::decay_t<decltype(argl)>;
			std::visit([&](auto&& argr) {
				using Tr = std::decay_t<decltype(argr)>;
				if constexpr (std::is_same_v<Tl, const PropertyOldVariant::InvalidType>) {
					if constexpr (std::is_same_v<Tr, const PropertyOldVariant::InvalidType>) {
						returnval = true; // both invalid
					}
					else {
						returnval = false; // one invalid
					}
				}
				else {
					if constexpr (std::is_same_v<Tr, const PropertyOldVariant::InvalidType>) {
						returnval = false;
					}
					else {
						//returnval = ((std::get<Tl>(lhs.val)) == (std::get<Tr>(rhs.val)));
					}
				}
				}, rhs.val);
			}, lhs.val);
	}

	template <class tpPropertyOldVariantType>
	friend bool operator>(const PropertyOldVariant& lhs, const tpPropertyOldVariantType& rhs) {
		return (lhs.getVal<tpPropertyOldVariantType>() > rhs);
	}

	template <class tpPropertyOldVariantType>
	friend bool operator>(const tpPropertyOldVariantType& lhs, const PropertyOldVariant& rhs) {
		return (lhs > rhs.getVal<tpPropertyOldVariantType>());
	}

	template <class tpPropertyOldVariantType>
	friend bool operator<(const PropertyOldVariant& lhs, const tpPropertyOldVariantType& rhs) {
		return (lhs.getVal<tpPropertyOldVariantType>() < rhs);
	}

	template <class tpPropertyOldVariantType>
	friend bool operator<(const tpPropertyOldVariantType& lhs, const PropertyOldVariant& rhs) {
		return (lhs < rhs.getVal<tpPropertyOldVariantType>());
	}

	//template <class tpPropertyOldVariantType>
	//friend bool operator+(const PropertyOldVariant& lhs, const tpPropertyOldVariantType& rhs) {
	//	return (lhs.getVal<tpPropertyOldVariantType>() + rhs);
	//}

	//template <class tpPropertyOldVariantType>
	//friend bool operator+(const tpPropertyOldVariantType& lhs, const PropertyOldVariant& rhs) {
	//	return (lhs + rhs.getVal<tpPropertyOldVariantType>());
	//}

	//template <class tpPropertyOldVariantType>
	//friend bool operator-(const PropertyOldVariant& lhs, const tpPropertyOldVariantType& rhs) {
	//	return (lhs.getVal<tpPropertyOldVariantType>() - rhs);
	//}

	//template <class tpPropertyOldVariantType>
	//friend bool operator/(const PropertyOldVariant& lhs, const tpPropertyOldVariantType& rhs) {
	//	return (lhs.getVal<tpPropertyOldVariantType>() / rhs);
	//}

	//template <class tpPropertyOldVariantType>
	//friend bool operator*(const PropertyOldVariant& lhs, const tpPropertyOldVariantType& rhs) {
	//	return (lhs.getVal<tpPropertyOldVariantType>() * rhs);
	//}

private:
	prop_var_t val;

	template <class tpPropertyOldVariantType>
	void initPropertyOldVariant(const tpPropertyOldVariantType& init) {
		// Can probably move this all into initializer list
		if constexpr ((std::is_same_v<tpPropertyOldVariantType, bool      >) ||
			(std::is_same_v<tpPropertyOldVariantType, int                 >) ||
			(std::is_same_v<tpPropertyOldVariantType, long                >) ||
			(std::is_same_v<tpPropertyOldVariantType, float               >) ||
			(std::is_same_v<tpPropertyOldVariantType, double              >) ||
			(std::is_same_v<tpPropertyOldVariantType, std::string         >) ||
			(std::is_same_v<tpPropertyOldVariantType, PropertyOldVariantContainer   >))
		{
			val = init;
			//std::cout << "Should initialize to type " << _getContainerizedTypeName<tpPropertyOldVariantType>() << "\n";
			//std::cout << "Initialized PropertyOldVariant to '" << _getVarTypeName(val) << "' with value '" << _getVarTypeVal(val) << "'\n";
		}
		else {
			//val = AnyContainer{ init };
			//std::cout << "Initialized PropertyOldVariant to custom type.\n";
			std::cerr << "PropertyOldVariant initialization error: trying to initialize PropertyOldVariant to unsupported type\n";
			val = InvalidType();
		}
	}

	template <class T>
	static std::string _getContainerizedTypeName() {
		if      constexpr (std::is_same_v<T, bool               >) return "int";
		else if constexpr (std::is_same_v<T, int                >) return "int";
		else if constexpr (std::is_same_v<T, long               >) return "long";
		else if constexpr (std::is_same_v<T, float              >) return "float";
		else if constexpr (std::is_same_v<T, double             >) return "double";
		else if constexpr (std::is_same_v<T, std::string        >) return "string";
		else if constexpr (std::is_same_v<T, PropertyOldVariantContainer  >) return "PropertyOldVariant container";
		else if constexpr (std::is_same_v<T, InvalidType	    >) return "invalid type";
		else													   return "unknown type";
	}

	static std::string _getVarTypeName(prop_var_t var) {
		std::string retstr;
		std::visit([&](auto&& arg) {
			using T = std::decay_t<decltype(arg)>;
			retstr = _getContainerizedTypeName<T>();
			}, var);
		return retstr;
	}

	static std::string _getVarTypeVal(prop_var_t var) {
		std::string retstr;
		std::visit([&](auto&& arg) {
			using T = std::decay_t<decltype(arg)>;
			if constexpr (std::is_same_v<T, PropertyOldVariantContainer>) {
				retstr = "unknown content";
			}
			else if constexpr (std::is_same_v<T, InvalidType>) {
				retstr = "invalid";
			}
			else {
				std::stringstream ss;
				ss << arg;
				retstr = ss.str();
			}
			}, var);
		return retstr;
	}

	PropertyOldVariant() {}
};

template <class tpPropertyOldVariantType>
PropertyOldVariant makePropertyOldVariant(const tpPropertyOldVariantType& val) {
	PropertyOldVariant newProp;
	newProp.initPropertyOldVariant<tpPropertyOldVariantType>(val);
	return newProp;
}

template <class tpPropertyOldVariantType>
PropertyOldVariant makePropertyOldVariant() {
	PropertyOldVariant newProp;
	newProp.initPropertyOldVariant<tpPropertyOldVariantType>(tpPropertyOldVariantType());
	return newProp;
}