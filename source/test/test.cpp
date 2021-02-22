#include "pch.h"
#include "Utilogeny/source/core/sdl/sdl_integration.h"
#include "Utilogeny/source/core/audio/audio_test.h"

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



