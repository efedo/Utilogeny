#include "WindowingSystem.h"
#include "../ExtendedException.h"

WindowingSystem m_WindowingSystem = WindowingSystem::SDL;
bool m_LockedWindowingSystem = false;

WindowingSystem getWindowingSystem() {
    return m_WindowingSystem;
}

bool WindowSDL() {
    return (getWindowingSystem() == WindowingSystem::SDL);
}

bool WindowGLFW() {
    return (getWindowingSystem() == WindowingSystem::GLFW);
}

void setWindowingSystem(WindowingSystem windowingSystem) {
    if (m_LockedWindowingSystem) THROW_EXTENDED_EXCEPTION("Tried to reset the Doodad windowing system");
    m_WindowingSystem = windowingSystem;
    m_LockedWindowingSystem = true;
}

void lockWindowingSystem() {
    m_LockedWindowingSystem = true;
}