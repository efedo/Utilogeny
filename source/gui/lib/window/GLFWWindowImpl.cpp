#include "Utilogeny/source/gui/lib/renderer/opengl/OpenGLRenderer.h"
#include "Utilogeny/source/gui/lib/renderer/bgfx/BgfxRenderer.h"
#include "GLFWWindowImpl.h"
#include "../ExtendedException.h"
#include <string>
#include <cassert>
#include "spdlog/spdlog.h"

# if defined(__APPLE__)
    # define GLFW_EXPOSE_NATIVE_COCOA
# elif (defined(linux) || defined(__linux) || defined(__linux__))
    # define GLFW_EXPOSE_NATIVE_X11
# elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
    # define GLFW_EXPOSE_NATIVE_WIN32
# endif
#include <GLFW/glfw3native.h>

std::map<GLFWwindow*, GLFWWindowImpl*> glfwWindowMap; // map of all existing GLFW windows (to direct events)

void GLFWWindowImpl::init_handle()
{
    const int platform = glfwGetPlatform();
    auto _window = this->getGlfwWindow();
//#if defined(_GLFW_WAYLAND)
//        wl_egl_window* win_impl = (wl_egl_window*)glfwGetWindowUserPointer(_window);
//        if (!win_impl)
//        {
//            int width, height;
//            glfwGetWindowSize(_window, &width, &height);
//            struct wl_surface* surface = (struct wl_surface*)glfwGetWaylandWindow(_window);
//            if (!surface)
//                m_handle =  nullptr;
//            win_impl = wl_egl_window_create(surface, width, height);
//            glfwSetWindowUserPointer(_window, (void*)(uintptr_t)win_impl);
//        }
//        m_handle = (void*)(uintptr_t)win_impl;
#	if defined(__APPLE__)
        if (platform != GLFW_PLATFORM_COCOA) spdlog::critical("Platform detection mismatch when obtaining native window handle");
        m_handle = glfwGetCocoaWindow(_window);
#   elif (defined(linux) || defined(__linux) || defined(__linux__))
        if (platform != GLFW_PLATFORM_X11) spdlog::critical("Platform detection mismatch when obtaining native window handle");
        m_handle = (void*)(uintptr_t)glfwGetX11Window(_window);
#	elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
        if (platform != GLFW_PLATFORM_WIN32) spdlog::critical("Platform detection mismatch when obtaining native window handle");
        m_handle = glfwGetWin32Window(_window);
#	endif
}

// GLFWWindowImpl constructor
GLFWWindowImpl::GLFWWindowImpl(Window* window, const std::string& title, int width, int height)
    : WindowImpl(window, title, width, height) 
{
    spdlog::info("Launching GLFW window implementation");
}

// GLFWWindowImpl destructor
GLFWWindowImpl::~GLFWWindowImpl() {
    // Need to lock Sdl/Glfw to prevent multithreaded access
    auto windowMngLock = lockWindowManagerContext(m_title);

    glfwDestroyWindow(m_glfw_window);
    glfwTerminate();
}

void GLFWWindowImpl::init() {
    {
        auto windowMngLock = lockWindowManagerContext(m_title);  // Need to lock Sdl/Glfw to prevent
        // multithreaded access

        glfwSetErrorCallback(glfwErrorCallback);

        // Initialize the GLFW window...
        if (!glfwInit()) {
            THROW_EXTENDED_EXCEPTION("Failed to initialize GLFW");
        }
    }

    if (m_renderer_type == RendererType::SDL) {
        THROW_EXTENDED_EXCEPTION("Cannot use SDL renderer in GLFW window");
    }
    else if (m_renderer_type == RendererType::OpenGL) {
        initOpenGL();
    }
    else if (m_renderer_type == RendererType::BGFX) {
        initBGFX();
    }
    else if (m_renderer_type == RendererType::LVGL) {
        THROW_EXTENDED_EXCEPTION("LVGL not currently supported with GLFW");
    }
    else if (m_renderer_type == RendererType::none) {
        initEmpty();
    } else {
        THROW_EXTENDED_EXCEPTION("Unrecognized renderer");
    }

    {
        auto windowMngLock = lockWindowManagerContext(m_title);
        // Associate native window with window implementation
        glfwSetWindowUserPointer(m_glfw_window, this);

        glfwMakeContextCurrent(m_glfw_window);
        glfwSwapInterval(1); // Enable vsync

        // Register the callbacks
        // Needs to be done for Imgui init
        // https://github.com/ocornut/imgui/issues/5003
        glfwSetKeyCallback(m_glfw_window, glfwKeyCallback);
        glfwSetCharCallback(m_glfw_window, glfwCharCallback);
        glfwSetMouseButtonCallback(m_glfw_window, glfwMouseButtonCallback);
        glfwSetCursorPosCallback(m_glfw_window, glfwCursorPosCallback);
        glfwSetScrollCallback(m_glfw_window, glfwScrollCallback);
        glfwSetFramebufferSizeCallback(m_glfw_window, glfwFramebufferSizeCallback);
        glfwSetWindowCloseCallback(m_glfw_window, glfwWindowCloseCallback);
        glfwSetWindowIconifyCallback(m_glfw_window, glfwWindowIconifyCallback);
        glfwSetWindowMaximizeCallback(m_glfw_window, glfwWindowMaximizeCallback);
        glfwSetWindowPosCallback(m_glfw_window, glfwWindowPosCallback);
        glfwSetWindowFocusCallback(m_glfw_window, glfwWindowFocusCallback);
        glfwSetWindowRefreshCallback(m_glfw_window, glfwWindowRefreshCallback);
        //  glfwSetJoystickCallback(glfwJoystickCallback);

        //// Initialize joystick and game controller
        //for (int i = GLFW_JOYSTICK_1; i <= GLFW_JOYSTICK_LAST; i++) {
        //    if (glfwJoystickPresent(i)) {
        //        const char* name = glfwGetJoystickName(i);
        //        // Process joystick device added event
        //    }
        //    if (glfwJoystickIsGamepad(i)) {
        //        const char* name = glfwGetGamepadName(i);
        //        // Process gamepad device added event
        //    }
        //}
    }
}

void GLFWWindowImpl::createNative() {
    {
        m_glfw_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
        if (!m_glfw_window) {
            glfwTerminate();
            THROW_EXTENDED_EXCEPTION("Failed to create GLFW window");
        }
        init_handle(); // populates native window handle
    }
}

#define GLFW_EXPOSE_NATIVE_WGL
#define GLFW_EXPOSE_NATIVE_WIN32 
#include <GLFW/glfw3native.h>
void GLFWWindowImpl::initOpenGL() {
    auto windowMngLock = lockWindowManagerContext(m_title);
    // Decide GL+GLSL versions
    #if defined(IMGUI_IMPL_OPENGL_ES2)
        // GL ES 2.0 + GLSL 100
        std::string glsl_version = "#version 100";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    #elif defined(__APPLE__)
        // GL 3.2 + GLSL 150
        std::string glsl_version = "#version 150";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
    #else
        // GL 3.0 + GLSL 130
        std::string glsl_version = "#version 130";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
        //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
    #endif

    //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE); // Necessary for deprecated OpenGL functions
    //// GLFW_OPENGL_CORE_PROFILE

    //m_glfw_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
    //if (!m_glfw_window) {
    //    glfwTerminate();
    //    THROW_EXTENDED_EXCEPTION("Failed to create GLFW window");
    //}
    //init_handle(); // populates native window handle

    createNative();

    //HWND hWnd = glfwGetWin32Window(m_glfw_window);

    // Create renderer
    // Should decide which renderer you want to use here
    m_renderer = std::make_unique<OpenGLRenderer>(getWindow(), glsl_version);
    m_renderer->makeCurrent();
    //std::unique_lock<std::mutex> contextLock = m_renderer->lockContext();  // Need to lock context
    m_renderer->initialize();
}

void GLFWWindowImpl::initBGFX() {

    {
        auto windowMngLock = lockWindowManagerContext(m_title);
        createNative();
    }

    //m_glfw_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
    //if (!m_glfw_window) {
    //    glfwTerminate();
    //    THROW_EXTENDED_EXCEPTION("Failed to create GLFW window");
    //}
    //init_handle(); // populates native window handle

    // Create renderer
    // Should decide which renderer you want to use here
    m_renderer = std::make_unique<BGFXRenderer>(getWindow());
    m_renderer->makeCurrent();
    //std::unique_lock<std::mutex> contextLock = m_renderer->lockContext();  // Need to lock context
    m_renderer->initialize();
}

void GLFWWindowImpl::initEmpty() {
    // todo: see SDL version, move visibilty control
    auto windowMngLock = lockWindowManagerContext(m_title);
    glfwWindowHint(GLFW_VISIBLE, 0);
    createNative();
    glfwWindowHint(GLFW_VISIBLE, 1);
}

// Getters

GLFWwindow* GLFWWindowImpl::getGlfwWindow() const {
    assert(m_glfw_window);
    return m_glfw_window;
}

//void* GLFWWindowImpl::getNativeHandle() const {
//    #if defined(_GLFW_WIN32)
//        return glfwGetWin32Window(m_glfw_window);
//    #elif defined (_GLFW_COCOA)
//        return glfwGetCocoaWindow(m_glfw_window);
//    #elif defined(_GLFW_X11)
//        return glfwGetX11Window(m_glfw_window);
//    #elif defined(_GLFW_WAYLAND)
//        return glfwGetWaylandWindow (m_glfw_window)
//    #endif
//}

//WindowingSystem GLFWWindowImpl::getWindowingSystem() const {
//    return WindowingSystem::GLFW;
//}

GLFWWindowImpl* GLFWWindowImpl::getCallingWindow(GLFWwindow* window) {       
    return (GLFWWindowImpl*)glfwGetWindowUserPointer(window);
}

// Setters
void GLFWWindowImpl::setTitleUnsafe(char * title) {
    // Need to lock Sdl/Glfw to prevent multithreaded access
    auto windowMngLock = lockWindowManagerContext(m_title);
    glfwSetWindowTitle(m_glfw_window, title);
}

void GLFWWindowImpl::setSizeUnsafe(int width, int height) {
    // Need to lock Sdl/Glfw to prevent multithreaded access
    auto windowMngLock = lockWindowManagerContext(m_title);
    glfwSetWindowSize(m_glfw_window, width, height);
}

bool GLFWWindowImpl::pollEvent(Event& event) {
    glfwGetEvents();
    return WindowImpl::pollEvent(event);
}

void GLFWWindowImpl::glfwGetEvents() { // static for safety
    // Need to lock Sdl/Glfw to prevent multithreaded access
    auto windowMngLock = lockWindowManagerContext(m_title);
    glfwPollEvents(); // Will call associated callbacks, which will populate eventQueue
}

void GLFWWindowImpl::glfwErrorCallback(int error, const char* description)
{
    //// Handle error
    //fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

void GLFWWindowImpl::glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Event event;
    if (action == GLFW_PRESS) {
        event.type = Event::Type::KeyDown;
    }
    else if (action == GLFW_RELEASE) {
        event.type = Event::Type::KeyUp;
    }
    else {
        return;
    }

    event.key.keycode = key;
    event.key.scancode = scancode;
    event.key.mod = mods;

    getCallingWindow(window)->eventQueue.push(event);
}

void GLFWWindowImpl::glfwCharCallback(GLFWwindow* window, unsigned int codepoint) {
    Event event;
    event.type = Event::Type::TextInput;
    snprintf(event.textInput.text, sizeof(event.textInput.text), "%c", codepoint);
    event.textInput.text[sizeof(event.textInput.text) - 1] = '\0';

    getCallingWindow(window)->eventQueue.push(event);
}

void GLFWWindowImpl::glfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    Event event;
    if (action == GLFW_PRESS) {
        event.type = Event::Type::MouseButtonDown;
    }
    else if (action == GLFW_RELEASE) {
        event.type = Event::Type::MouseButtonUp;
    }
    else {
        return;
    }

    double x, y;
    glfwGetCursorPos(window, &x, &y);
    event.mouseButton.x = static_cast<int>(x);
    event.mouseButton.y = static_cast<int>(y);
    event.mouseButton.button = button;

    getCallingWindow(window)->eventQueue.push(event);
}

void GLFWWindowImpl::glfwCursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    Event event;
    event.type = Event::Type::MouseMotion;
    event.mouseMotion.x = static_cast<int>(xpos);
    event.mouseMotion.y = static_cast<int>(ypos);

    getCallingWindow(window)->eventQueue.push(event);
}

void GLFWWindowImpl::glfwScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    Event event;
    event.type = Event::Type::MouseWheel;
    event.mouseWheel.x = static_cast<int>(xoffset);
    event.mouseWheel.y = static_cast<int>(yoffset);

    getCallingWindow(window)->eventQueue.push(event);
}

void GLFWWindowImpl::glfwFramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    Event event;
    event.type = Event::Type::WindowResize;
    event.windowResize.width = width;
    event.windowResize.height = height;

    getCallingWindow(window)->eventQueue.push(event);
}

void GLFWWindowImpl::glfwWindowCloseCallback(GLFWwindow* window) {
    Event event;
    event.type = Event::Type::Quit;

    getCallingWindow(window)->eventQueue.push(event);
}

void GLFWWindowImpl::glfwWindowPosCallback(GLFWwindow* window, int x, int y) {
    Event event;
    event.type = Event::Type::WindowMoved;
    event.windowMoved.x = x;
    event.windowMoved.y = y;
    getCallingWindow(window)->eventQueue.push(event);
}

void GLFWWindowImpl::glfwWindowFocusCallback(GLFWwindow* window, int focused) {
    Event event;
    event.type = focused ? Event::Type::WindowFocusGained : Event::Type::WindowFocusLost;
    event.windowFocus.focused = static_cast<bool>(focused);
    getCallingWindow(window)->eventQueue.push(event);
}

void GLFWWindowImpl::glfwWindowRefreshCallback(GLFWwindow* window) {
    Event event;
    event.type = Event::Type::WindowRefresh;
    getCallingWindow(window)->eventQueue.push(event);
}

void GLFWWindowImpl::glfwWindowIconifyCallback(GLFWwindow* window, int iconified) {
    Event event;
    event.type = iconified ? Event::Type::WindowIconified : Event::Type::WindowRestored;
    event.windowIconified.iconified = static_cast<bool>(iconified);
    getCallingWindow(window)->eventQueue.push(event);
}

void GLFWWindowImpl::glfwWindowMaximizeCallback(GLFWwindow* window, int maximized) {
    Event event;
    event.type = Event::Type::WindowMaximized;
    event.windowMaximized.maximized = static_cast<bool>(maximized);
    getCallingWindow(window)->eventQueue.push(event);
}

//void GLFWWindowImpl::glfwJoystickCallback(int joy, int event) {
//    if (event == GLFW_CONNECTED) {
//        // The joystick was connected
//        Event e;
//        e.type = Event::Type::JoystickDeviceAdded;
//        e.joystickDevice.deviceIndex = joy;
//        eventQueue.push(e);
//    }
//    else if (event == GLFW_DISCONNECTED) {
//        // The joystick was disconnected
//        Event e;
//        e.type = Event::Type::JoystickDeviceRemoved;
//        e.joystickDevice.deviceIndex = joy;
//        eventQueue.push(e);
//    }
//}