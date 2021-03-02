#include "property.h"

Property PropertyContainer::dummyBuiltin(std::string("Shared placeholder property"));

class TestContainerDog : public PropertyContainer {
public:

	Property bark_freq = Property(int());
	
	virtual void registerBuiltinProperties() {
		registerBuiltinProperty("bark_freq", this->bark_freq);
	};
};

void testFunc() {
	TestContainerDog dog;
	dog.bark_freq = 5;
	dog.property("bark_freq") = 5;// .property("freq") = 5;
	//PropertyContainer propcontainer = dog.property("bark");

	//dog.property("bark").property("freq") = 5;
	//dog.property("bark").get<PropertyContainer>().property("freq").get<int>() = 5;
	//dog.property("bark").get<PropertyContainer>().property("freq") = 5;
}

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
