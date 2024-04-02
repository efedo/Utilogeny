#pragma once

#include <string>
#include <memory>
#include <mutex>
#include "WindowingSystem.h"
#include "../renderer/RendererTypes.h"
//#include "../event/event.h"
//#include <thread>
#include "../renderer/Color.h"
//#include <future>
#include "../ExecutableObject.h"
#include "../Application.h"

// Forward declaration of the implementation class
class WindowImpl;
struct SDL_Window;
struct GLFWwindow;
class Window : public ExecutableObject {
public:
    // Constructor
    Window(const std::string& title, int width, int height, bool no_renderer = false);

    // Destructor
    virtual ~Window();

    // Non-copyable
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    // Setters
    void setTitle(const std::string& title);
    void setSize(int width, int height); // change to uint32_t
    void setBackColor(RGBA8 color);
    void setPos(uint32_t x, uint32_t y);
    void setBordered(bool bordered);
    void setFullscreen(bool fullscreen);
    //void setWindowFlags(SDL_Window* window, uint32_t flags);

    // Getters
    std::string getTitle() const;
    int getWidth() const;
    int getHeight() const;
    SDL_Window* getSdlWindow() const;
    GLFWwindow* getGlfwWindow() const;
    void* getNativeHandle() const;
    //void* getNativeHandle() const;

protected:
    WindowImpl * getImpl() const; // Note: shadowing rather than virtual
};