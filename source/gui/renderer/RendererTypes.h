#pragma once

enum class RendererType : int {
	none = 0,
	any = 1,
	SDL,
	// SDL_WINDOW_VULKAN
	// SDL_WINDOW_METAL
	// SDL_WINDOW_OPENGL
	// GDI
	// Xlib
	// OpenGL ES
	OpenGL,
	Vulkan,
	Metal,
	DirectX,
	BGFX,
	LVGL
};

RendererType getRendererType();
void setRendererType(RendererType); // Can only be set once before window creation
void lockRendererType();
extern RendererType m_RendererType;
extern bool m_LockedRendererType;