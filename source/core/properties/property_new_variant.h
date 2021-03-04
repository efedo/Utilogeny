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

class PropertyContainer;

class PropertyNewVariant {
public:

	struct UninitializedType {};

	struct InvalidType {};

	template <class tpType>
	struct DynamicValue {
		DynamicValue(tpType* newloc) : location(newloc) {}
		static constexpr bool isDynamic() { return true; }
		using ContainedType = tpType;
		tpType* location;
	};

	template <class tpType>
	struct StaticValue {
		StaticValue(tpType* newloc) : location(newloc) {}
		static constexpr bool isDynamic() { return false; }
		using ContainedType = tpType;
		tpType* location;
	};

	//using prop_var_t = std::variant<InvalidType, bool *, int *, float *, double *, std::string *>; //AnyContainer

	using prop_var_t = std::variant<
		UninitializedType,
		InvalidType,
		StaticValue<bool>,
		StaticValue<int>,
		StaticValue<float>,
		StaticValue<double>,
		StaticValue<std::string>,
		StaticValue<PropertyContainer>,
		DynamicValue<bool>,
		DynamicValue<int>,
		DynamicValue<float>,
		DynamicValue<double>,
		DynamicValue<std::string>,
		DynamicValue<PropertyContainer>
	>;

	std::string getTypeString() {
		return _getVarTypeName(val);
	}

	std::string getValString() {
		return _getVarTypeVal(val);
	}

	PropertyNewVariant& operator=(const PropertyNewVariant& rhs)
	{
		if (val.index() == rhs.val.index()) {
			val = rhs.val;
		}
		else {
			std::cerr << "Property assignment error: Property of type '" << _getVarTypeName(rhs.val) << "' cannot be copied to existing PropertyNewVariant of type '" << _getVarTypeName(val) << "'\n";
		}
		return *this;
	}

	template <class tpPropertyNewVariantType>
	PropertyNewVariant& operator=(const tpPropertyNewVariantType& rhs)
	{
		// https://en.cppreference.com/w/cpp/utility/variant/visit
		// Check that rhs is convertible to current type
		std::visit([&](auto&& arg) {
			using T = std::decay_t<decltype(arg)>;
			if constexpr (std::is_same_v<T, InvalidType>) {
				return "invalid";
			}
			else if constexpr (std::is_same_v<T, UninitializedType>) {
				return "uninitialized";
			}
			else {
				using TC = T::ContainedType;

				// Override to prevent implicit conversion of floating point to integer types
				if constexpr (std::is_convertible_v<tpPropertyNewVariantType, TC> && !(std::is_floating_point_v<tpPropertyNewVariantType> && std::is_integral_v<TC>)) {
					val = (TC)(rhs);
				}
				else {
					std::cerr << "Property assignment error: Value of type '" << _getTypeName<tpPropertyNewVariantType>() <<
						"' cannot be assigned to existing property of type '" << _getTypeName<TC>() << "'\n";
				}
			}
		}, val);

		//std::cout << "PropertyNewVariant now set to '" << _getVarTypeName(val) << "' with value '" << _getVarTypeVal(val) << "'\n";
		return *this;
	}

	template <class tpPropertyNewVariantType>
	operator tpPropertyNewVariantType() const {
		return getVal<tpPropertyNewVariantType>();
	}

	template <class tpPropertyNewVariantType>
	tpPropertyNewVariantType getVal() const {
		tpPropertyNewVariantType returnval;
		std::visit([&](auto&& arg) {
			using T = std::decay_t<decltype(arg)>;
			// Override to prevent implicit conversion of floating point to integer types
			if constexpr (std::is_convertible_v<T, tpPropertyNewVariantType>) {
				returnval = (tpPropertyNewVariantType)(std::get<T>(val));
			}
			else {
				std::cerr << "PropertyNewVariant value not convertible to desired type!\n";
			}
			}, val);
		return returnval;
	}

	template <class tpPropertyNewVariantType>
	friend bool operator==(const PropertyNewVariant& lhs, const tpPropertyNewVariantType& rhs) {
		return (lhs.getVal<tpPropertyNewVariantType>() == rhs);
	}

	template <class tpPropertyNewVariantType>
	friend bool operator==(const tpPropertyNewVariantType& lhs, const PropertyNewVariant& rhs) {
		return (lhs == rhs.getVal<tpPropertyNewVariantType>());
	}

	friend bool operator==(const PropertyNewVariant& lhs, const PropertyNewVariant& rhs) {
		bool returnval;
		std::visit([&](auto&& argl) {
			using Tl = std::decay_t<decltype(argl)>;
			std::visit([&](auto&& argr) {
				using Tr = std::decay_t<decltype(argr)>;
				if constexpr (std::is_same_v<Tl, const PropertyNewVariant::InvalidType>) {
					if constexpr (std::is_same_v<Tr, const PropertyNewVariant::InvalidType>) {
						returnval = true; // both invalid
					}
					else {
						returnval = false; // one invalid
					}
				}
				else {
					if constexpr (std::is_same_v<Tr, const PropertyNewVariant::InvalidType>) {
						returnval = false;
					}
					else {
						//returnval = ((std::get<Tl>(lhs.val)) == (std::get<Tr>(rhs.val)));
					}
				}
				}, rhs.val);
			}, lhs.val);
	}

	template <class tpPropertyNewVariantType>
	friend bool operator>(const PropertyNewVariant& lhs, const tpPropertyNewVariantType& rhs) {
		return (lhs.getVal<tpPropertyNewVariantType>() > rhs);
	}

	template <class tpPropertyNewVariantType>
	friend bool operator>(const tpPropertyNewVariantType& lhs, const PropertyNewVariant& rhs) {
		return (lhs > rhs.getVal<tpPropertyNewVariantType>());
	}

	template <class tpPropertyNewVariantType>
	friend bool operator<(const PropertyNewVariant& lhs, const tpPropertyNewVariantType& rhs) {
		return (lhs.getVal<tpPropertyNewVariantType>() < rhs);
	}

	template <class tpPropertyNewVariantType>
	friend bool operator<(const tpPropertyNewVariantType& lhs, const PropertyNewVariant& rhs) {
		return (lhs < rhs.getVal<tpPropertyNewVariantType>());
	}

	//template <class tpPropertyNewVariantType>
	//friend bool operator+(const PropertyNewVariant& lhs, const tpPropertyNewVariantType& rhs) {
	//	return (lhs.getVal<tpPropertyNewVariantType>() + rhs);
	//}

	//template <class tpPropertyNewVariantType>
	//friend bool operator+(const tpPropertyNewVariantType& lhs, const PropertyNewVariant& rhs) {
	//	return (lhs + rhs.getVal<tpPropertyNewVariantType>());
	//}

	//template <class tpPropertyNewVariantType>
	//friend bool operator-(const PropertyNewVariant& lhs, const tpPropertyNewVariantType& rhs) {
	//	return (lhs.getVal<tpPropertyNewVariantType>() - rhs);
	//}

	//template <class tpPropertyNewVariantType>
	//friend bool operator/(const PropertyNewVariant& lhs, const tpPropertyNewVariantType& rhs) {
	//	return (lhs.getVal<tpPropertyNewVariantType>() / rhs);
	//}

	//template <class tpPropertyNewVariantType>
	//friend bool operator*(const PropertyNewVariant& lhs, const tpPropertyNewVariantType& rhs) {
	//	return (lhs.getVal<tpPropertyNewVariantType>() * rhs);
	//}

private:
	prop_var_t val;

	template <class tpPropertyType>
	friend PropertyNewVariant registerStackNewProperty(tpPropertyType& property);

	template <class tpPropertyType>
	friend PropertyNewVariant getDynamicNewProperty();

	template <class tpPropertyType>
	friend PropertyNewVariant getDynamicNewProperty(const tpPropertyType&);

	template <class tpPropertyNewVariantType>
	static constexpr bool canHoldType() {
		if constexpr ((std::is_same_v<tpPropertyNewVariantType, bool      >) ||
			(std::is_same_v<tpPropertyNewVariantType, int                 >) ||
			(std::is_same_v<tpPropertyNewVariantType, long                >) ||
			(std::is_same_v<tpPropertyNewVariantType, float               >) ||
			(std::is_same_v<tpPropertyNewVariantType, double              >) ||
			(std::is_same_v<tpPropertyNewVariantType, std::string         >) ||
			(std::is_same_v<tpPropertyNewVariantType, PropertyContainer   >))
		{
			return true;
		}
		else {
			return false;
		}
	}

	template <class tpPropertyNewVariantType>
	void initStatic(tpPropertyNewVariantType * init) {
		// Can probably move this all into initializer list
		if constexpr (canHoldType<tpPropertyNewVariantType>())
		{
			val = StaticValue(init); //<tpPropertyNewVariantType>
		}
		else {
			std::cerr << "Property initialization error: trying to initialize property to unsupported static type\n";
			val = InvalidType();
		}
	}

	template <class tpPropertyNewVariantType>
	void initDynamic(const tpPropertyNewVariantType * init) {
		// Can probably move this all into initializer list
		if constexpr (canHoldType<tpPropertyNewVariantType>())
		{
			val = DynamicValue<tpPropertyNewVariantType>(init);
		}
		else {
			std::cerr << "Property initialization error: trying to initialize property to unsupported dynamic type\n";
			val = InvalidType();
		}
	}

	template <class TC>
	static constexpr std::string _getTypeName() {
		if      constexpr (std::is_same_v<TC, bool               >) return "int";
		else if constexpr (std::is_same_v<TC, int                >) return "int";
		else if constexpr (std::is_same_v<TC, long               >) return "long";
		else if constexpr (std::is_same_v<TC, float              >) return "float";
		else if constexpr (std::is_same_v<TC, double             >) return "double";
		else if constexpr (std::is_same_v<TC, std::string        >) return "string";
		else if constexpr (std::is_same_v<TC, PropertyContainer  >) return "Property container";
		else													   return "unknown type";
	}

	template <class T>
	static constexpr std::string _getContainerizedTypeName() {
		if constexpr (std::is_same_v<T, InvalidType>) {
			return "invalid";
		}
		else if constexpr (std::is_same_v<T, UninitializedType>) {
			return "uninitialized";
		}
		else {
			using TC = T::ContainedType;
			return _getTypeName<TC>();
		}
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
			if constexpr (std::is_same_v<T, InvalidType>) {
				retstr = "invalid";
			}
			else if constexpr (std::is_same_v<T, UninitializedType>) {
				retstr = "invalid";
			}
			else {
				using TContained = T::ContainedType;
				if constexpr (std::is_same_v<TContained, PropertyContainer>) {
					retstr = "container content";
				}
				else {
					std::stringstream ss;
					ss << *(arg.location);
					retstr = ss.str();
				}
			}
			}, var);
		return retstr;
	}

	PropertyNewVariant() {}
};

template <class tpPropertyType>
PropertyNewVariant registerStackNewProperty(tpPropertyType& prop) {
	PropertyNewVariant newProp;
	newProp.initStatic<tpPropertyType>(&prop);
	return newProp;
}

template <class tpPropertyType>
PropertyNewVariant getDynamicNewProperty() {
	PropertyNewVariant newProp;
	newProp.initDynamic<tpPropertyType>(new tpPropertyType());
	return newProp;
}

template <class tpPropertyType>
PropertyNewVariant getDynamicNewProperty(const tpPropertyType& val) {
	PropertyNewVariant newProp;
	newProp.initDynamic<tpPropertyNewVariantType>(new tpPropertyType(val));
	return newProp;
}