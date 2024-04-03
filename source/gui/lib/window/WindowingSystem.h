#pragma once

enum class WindowingSystem : int { SDL = 1, GLFW = 2 };

WindowingSystem getWindowingSystem();
bool WindowSDL();
bool WindowGLFW();
void setWindowingSystem(WindowingSystem); // Can only be set once before window creation
void lockWindowingSystem();
extern WindowingSystem m_WindowingSystem;
extern bool m_LockedWindowingSystem;
