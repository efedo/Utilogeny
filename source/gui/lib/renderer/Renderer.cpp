#include "Renderer.h"
#include <GLFW/glfw3.h> // Not sure we want these here
#include <SDL.h>        // Not sure we want these here
#include "../window/Window.h"
#include <cassert>
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_glfw.h"
#include "spdlog/spdlog.h"

void * Renderer::m_currentWindow = nullptr; // active window
std::mutex Renderer::m_contextMutex; // context mutex

void Renderer::setBackColor(RGBA8 color) {
    backColor = color;
}

RGBA8 Renderer::getBackColor() {
    return backColor;
}

Renderer::Renderer(Window* window)
: m_window(window)
{
    assert(m_window);
    spdlog::info("Creating renderer base");
}

std::unique_lock<std::mutex> Renderer::lockContext() {
    // Lock the mutex to synchronize access to the OpenGL context
    std::unique_lock<std::mutex> lock(m_contextMutex);
    makeCurrent();
    return std::move(lock);
}

SDL_Window* Renderer::getSdlWindow() const {
    assert(m_window);
    return m_window->getSdlWindow();
}

GLFWwindow* Renderer::getGlfwWindow() const {
    assert(m_window);
    return m_window->getGlfwWindow();
}

//void* Renderer::getNativeHandle() const {
//    assert(m_window);
//    return m_window->getNativeHandle();
//}

void Renderer::registerImguiCallbacks() {
    if (WindowGLFW()) ImGui_ImplGlfw_InstallCallbacks(getGlfwWindow());
}

Window* Renderer::getWindow() const {
    return m_window;
}