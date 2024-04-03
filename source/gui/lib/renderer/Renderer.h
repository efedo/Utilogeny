// Renderer.h

// Renderers are not thread-safe, you need to lock the context from the calling window!

#pragma once

#include <cstdint>
#include <mutex>
#include "Color.h"

struct Rect {
    int x, y, w, h;
};

//class WindowImpl;
class Window;
class Event;
struct SDL_Window;
struct GLFWwindow;

enum class RendererTypes : uint8_t {
    OpenGL,
    BGFX,
    BasicOnly
};

class ImGuiContextManager;

class Renderer {
public:
    Renderer(Window * window); // Window* window
    virtual ~Renderer() = default;
    virtual void initialize() = 0;
    virtual void imguiInit() = 0;
    virtual void clear() = 0;
    virtual void render() = 0;
    virtual void renderImgui() = 0;
    virtual void swapBuffers() = 0;
    std::unique_lock<std::mutex> lockContext(); // todo: move to RenderereImpl base
    virtual void setBackColor(RGBA8 color);
    RGBA8 getBackColor();
    virtual void drawRect(Rect rect, RGBA8 color) = 0;
    void registerImguiCallbacks(); // todo: move to RenderereImpl base
    virtual void makeCurrent() = 0; // todo: move to derived RenderereImpl for direct renderers 
    Window * getWindow() const;
protected:
    //void * getNativeHandle() const;
    SDL_Window * getSdlWindow() const; // todo: move to RenderereImpl base
    GLFWwindow * getGlfwWindow() const; // todo: move to RenderereImpl base
    //virtual void getMouseState(int& x, int& y) = 0;
    //virtual void handleEvent(const Event* sdlEvent) = 0;
    RGBA8 backColor = RGBA8{ 0, 0, 0, 0 };
    //WindowImpl * m_windowImpl;
    Window * m_window;
    static void * m_currentWindow; // active window ... replace with Window pointer?
    static std::mutex m_contextMutex; // context mutex
    ImGuiContextManager* m_imguiContextMngr = 0;
};



