#pragma once

#include "window/WindowingSystem.h"

struct Settings {
	WindowingSystem defaultWindowingSystem = WindowingSystem::SDL;
	const unsigned int maxRefreshRate = 240;
};

extern Settings settings;