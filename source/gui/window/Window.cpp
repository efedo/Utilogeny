#include "Window.h"
#include "GLFWWindowImpl.h"
#include "SDLWindowImpl.h"
#include "../renderer/Renderer.h"
#include <thread>
#include <mutex>
#include <iostream>
#include <chrono>
#include "../settings.h"
#include "spdlog/spdlog.h"

// Constructor
// Fixme: not fully thread safe
Window::Window(const std::string& title, int width, int height, bool no_renderer)
{
    lockWindowingSystem();

    spdlog::info("Launching window \"" + title + "\"");

    if (getWindowingSystem() == WindowingSystem::SDL) {
        m_impl = new SDLWindowImpl(this, title, width, height);
    }
    else if (getWindowingSystem() == WindowingSystem::GLFW) {
        m_impl = new GLFWWindowImpl(this, title, width, height);
    }

    // Sane defaults, use SDL/OpenGL renderer
    if (getRendererType() == RendererType::any) {
        if (getWindowingSystem() == WindowingSystem::SDL) {
            setRendererType(RendererType::SDL);
        }
        else if (getWindowingSystem() == WindowingSystem::GLFW) {
            setRendererType(RendererType::OpenGL);
        }
    }
    
    if (no_renderer) {
        getImpl()->m_renderer_type = RendererType::none;
    }
    else {
        getImpl()->m_renderer_type = getRendererType();
    }
    exec(); // Only returns after init complete
}

// Destructor
Window::~Window() {
}

// Setters

void Window::setTitle(const std::string& title) {
    getImpl()->m_title = title;
    getImpl()->setTitle(title);
}

void Window::setSize(int width, int height) {
    getImpl()->m_width = width;
    getImpl()->m_height = height;
    getImpl()->setSize(width, height);
}

void Window::setBackColor(RGBA8 color) {
    getImpl()->setBackColor(color);
}

// Getters

WindowImpl* Window::getImpl() const {
    return (WindowImpl*)m_impl;
}

//WindowingSystem Window::getWindowingSystem() const {
//    return getImpl()->getWindowingSystem();
//}

std::string Window::getTitle() const {
    return getImpl()->m_title;
}

int Window::getWidth() const {
    return getImpl()->m_width;
}

int Window::getHeight() const {
    return getImpl()->m_height;
}

SDL_Window* Window::getSdlWindow() const {
    return getImpl()->getSdlWindow();
}

GLFWwindow* Window::getGlfwWindow() const {
    return getImpl()->getGlfwWindow();
}

void* Window::getNativeHandle() const {
    return getImpl()->getNativeHandle();
}

//void* Window::getNativeHandle() const {
//    return getImpl()->getNativeHandle();
//}
