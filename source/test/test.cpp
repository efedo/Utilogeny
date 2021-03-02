#include "pch.h"
#include "Utilogeny/source/core/sdl/sdl_integration.h"
#include "Utilogeny/source/core/audio/audio_test.h"
#include "Utilogeny/source/core/properties/property.h"

TEST(TestCaseName, TestName) {
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}

TEST(SdlTests, SdlLaunchTest) {
	launchSDLwindow();
}

TEST(SoLoudTests, SoLoudInitTest) {
	audio_test();
}

TEST(PropertyTests, PropertySynopsisTest) {


	class TestContainerDog : public PropertyContainer {
	public:

		Property bark_freq = Property(int());

		virtual void registerBuiltinProperties() {
			registerBuiltinProperty("bark_freq", this->bark_freq);
		};
	};

	TestContainerDog dog;
	dog.bark_freq = 5;
	dog.property("bark_freq") = 5;// .property("freq") = 5;
	int test = dog.property("bark_freq");

	std::cout << "Property value was: " << test << "\n";
	//PropertyContainer propcontainer = dog.property("bark");

	//dog.property("bark").property("freq") = 5;
	//dog.property("bark").get<PropertyContainer>().property("freq").get<int>() = 5;
	//dog.property("bark").get<PropertyContainer>().property("freq") = 5;
}




