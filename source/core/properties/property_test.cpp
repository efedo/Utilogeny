#include "Utilogeny/source/test/pch.h"
#include "Utilogeny/source/core/properties/property.h"
#include "Utilogeny/source/core/properties/property_new_variant.h"

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

TEST(PropertyTests, StackPropertyTest) {

	bool test = true;
	PropertyNewVariant testprop = registerStackNewProperty(test);

	// Check initialization with unregistered type (should fail)
	cerr_off();
	class UnknownClassA {} unkcla;
	PropertyNewVariant unkclpn = registerStackNewProperty(unkcla);
	cerr_on();
	EXPECT_EQ(unkclpn.getTypeString(), "invalid");

	// Check initialization with built-in types
	bool stack_bool = true;
	PropertyNewVariant prop_bool = registerStackNewProperty(stack_bool);
	// Note that some compilers treat bool as an int
	EXPECT_TRUE((prop_bool.getTypeString() == "bool") || (prop_bool.getTypeString() == "int"));
	EXPECT_TRUE(prop_bool == true); // implicit value operator
	EXPECT_EQ(prop_bool.getVal<bool>(), true); // explicit value return

	Property test1 = getDynamicProperty<bool>();

	//Property prop_int = makeProperty<int>(4525);
	//EXPECT_EQ(prop_int.getTypeString(), "int");
	//EXPECT_TRUE(prop_int == 4525); // implicit value operator
	//EXPECT_EQ(prop_int.getVal<int>(), 4525); // explicit value return

	//Property prop_float = makeProperty<float>(-418.144f);
	//EXPECT_EQ(prop_float.getTypeString(), "float");
	//EXPECT_TRUE(prop_float == -418.144f); // implicit value operator
	//EXPECT_EQ(prop_float.getVal<float>(), -418.144f); // explicit value return

	//Property prop_double = makeProperty<double>(-418.144371920);
	//EXPECT_EQ(prop_double.getTypeString(), "double");
	//EXPECT_TRUE(prop_double == -418.144371920); // implicit value operator
	//EXPECT_EQ(prop_double.getVal<double>(), -418.144371920); // explicit value return

	//int x = 5;
	//double y = 6.5;

	//std::cout << "int 5 == double 6.5? :" << (x == y ? "yes" : "no") << "\n";

	//class TestContainerDog : public PropertyContainer {
	//public:

	//	Property bark_freq = makeProperty<uint64_t>();

	//	virtual void registerBuiltinProperties() {
	//		registerBuiltinProperty("bark_freq", this->bark_freq);
	//	};
	//};

	//TestContainerDog dog;
	//dog.bark_freq = 5;
	//dog.property("bark_freq") = 9;
	//dog.property("bark_freq") = 5.5;
	////dog.property("bark") = 5;


	////int test = dog.property("bark_freq");

	////std::cout << "Property value was: " << test << "\n";
	////PropertyContainer propcontainer = dog.property("bark");

	////dog.property("bark").property("freq") = 5;
	////dog.property("bark").get<PropertyContainer>().property("freq").get<int>() = 5;
	////dog.property("bark").get<PropertyContainer>().property("freq") = 5;

	//int blah = 52;
	//Property testregister(blah);
	//std::cout << "Returned " << testregister.getVal<int>() << "\n";
	//std::cout << "Returned " << testregister.getVal<double>() << "\n";
}




