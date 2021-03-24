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
	Property testprop = registerStackProperty(test);

	// Check initialization with unregistered type (should fail)
	cerr_off();
	class UnknownClassA {} unkcla;
	Property unkclpn = registerStackProperty(unkcla);
	cerr_on();
	EXPECT_EQ(unkclpn.getTypeString(), "invalid");

	// Check initialization with built-in types
	bool stack_bool = true;
	Property prop_bool = registerStackProperty(stack_bool);
	// Note that some compilers treat bool as an int
	EXPECT_TRUE((prop_bool.getTypeString() == "bool") || (prop_bool.getTypeString() == "int"));
	EXPECT_TRUE(prop_bool == true); // implicit value operator
	EXPECT_TRUE(true == prop_bool); // implicit value operator
	EXPECT_EQ(prop_bool.getVal<bool>(), true); // explicit value return
	//EXPECT_TRUE(prop_bool == prop_bool); // explicit value return

	int stack_int = 4525;
	Property prop_int = registerStackProperty(stack_int);
	EXPECT_TRUE(prop_int.getTypeString() == "int");
	EXPECT_TRUE(prop_int == 4525); // implicit value operator
	EXPECT_TRUE(4525 == prop_int); // implicit value operator
	EXPECT_EQ(prop_int.getVal<int>(), 4525); // explicit value return
	//EXPECT_TRUE(prop_int == prop_int); // explicit value return
	//EXPECT_FALSE(prop_bool == prop_int); // explicit value return

	float stack_float = -418.144f;
	Property prop_float = registerStackProperty(stack_float);
	EXPECT_TRUE(prop_float.getTypeString() == "float");
	EXPECT_TRUE(prop_float == -418.144f); // implicit value operator
	EXPECT_TRUE(-418.144f == prop_float); // implicit value operator
	EXPECT_EQ(prop_float.getVal<float>(), -418.144f); // explicit value return
	//EXPECT_TRUE(prop_float == prop_float); // explicit value return
	//EXPECT_FALSE(prop_float == prop_bool); // explicit value return
	//EXPECT_FALSE(prop_float == prop_int); // explicit value return

	double stack_double = -418.144371920;
	Property prop_double = registerStackProperty(stack_double);
	EXPECT_TRUE(prop_double.getTypeString() == "double");
	EXPECT_TRUE(prop_double == -418.144371920); // implicit value operator
	EXPECT_TRUE(-418.144371920 == prop_double); // implicit value operator
	//EXPECT_TRUE(-417.144371920 > prop_double); // implicit value operator
	//EXPECT_TRUE(-419.144371920 < prop_double); // implicit value operator
	//EXPECT_TRUE(prop_double < -417.144371920); // implicit value operator
	//EXPECT_TRUE(prop_double > -419.144371920); // implicit value operator
	EXPECT_EQ(prop_double.getVal<double>(), -418.144371920); // explicit value return
	//EXPECT_TRUE(prop_double == prop_double); // explicit value return
	//EXPECT_FALSE(prop_double == prop_float); // explicit value return
	//EXPECT_FALSE(prop_double == prop_bool); // explicit value return
	//EXPECT_FALSE(prop_double == prop_int); // explicit value return

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




