#pragma once

#include <string>
#include "../event/event.h"
#include "../renderer/RendererTypes.h"
#include "../renderer/Renderer.h"
#include <memory>
#include <queue>
#include <map>
#include "../ExecutableObjectImpl.h"
#include "WindowingSystem.h"

//
// PIMPL idiom
// Not really clear that this needs to be a separate class from base Window?
//

// WindowImpl classes should be thread-safe (mutex for both SDL/GLFW calls and for renderer back-end calls)

class Window;
class SDL_Window;
class GLFWwindow;

class WindowImpl : public ExecutableObjectImpl {
public:
    // Constructor
    WindowImpl(Window* window, const std::string& title, int width, int height);

    // Destructor
    virtual ~WindowImpl();

    // Rendering control
    void render() override;

    // Getters
    virtual SDL_Window* getSdlWindow() const;
    virtual GLFWwindow* getGlfwWindow() const;
    void* getNativeHandle() const;
    //virtual WindowingSystem getWindowingSystem() const = 0;
    Window* getWindow() const;


    // Setters
    void setTitle(const std::string& title);
    void setSize(int width, int height);
    virtual void setBackColor(RGBA8 color);

protected:
    friend class Window;

    virtual void processEvents();
    // Common properties
    Window* m_parentWindow;

    // Properties
    std::string m_title;
    int m_width;
    int m_height;
    void* m_handle = 0;
    virtual void init_handle();

    // Setters
    // Note: unsafe because must be called by exec loop thread or funky stuff happens,
    // particularly with GLFW
    virtual void setTitleUnsafe(char *) = 0;
    virtual void setSizeUnsafe(int width, int height) = 0;

    // Getters
    static std::unique_lock<std::mutex> lockWindowManagerContext(std::string caller);

    // Renderer object
    RendererType m_renderer_type = RendererType::any;
    std::unique_ptr<Renderer> m_renderer;
private:
    static std::mutex WndManagerMutex;
};