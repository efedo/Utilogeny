#pragma once

#include "ExecutableObject.h"
#include "window/WindowingSystem.h"
#include "renderer/RendererTypes.h"

class Doodad : public ExecutableObject {
public:
	Doodad();
	static WindowingSystem getWindowingSystem();
	static void setWindowingSystem(WindowingSystem); // Can only be set once before window creation
	static RendererType getRendererType();
	static void setRendererType(RendererType); // Can only be set once before window creation
private:
	friend class Window;
	static void lockWindowingSystem();
	static WindowingSystem m_WindowingSystem;
	static bool m_LockedWindowingSystem;
	static void lockRendererType();
	static RendererType m_RendererType;
	static bool m_LockedRendererType;
};

//namespace Doodad {
extern Doodad app;
//}