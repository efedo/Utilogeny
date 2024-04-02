#define SDL_MAIN_HANDLED
#include <SDL.h>
//#include <SDL_syswm.h>
#include <SDL_platform.h>
#include "source/renderer/opengl/OpenGLRenderer.h"
#include "source/renderer/sdl/SDLRenderer.h"
#include "source/renderer/bgfx/BgfxRenderer.h"
#ifdef ENABLE_LVGL
#include "source/renderer/lvgl/lvgltests.h"
#endif
#include "SDLWindowImpl.h"
#include "../ExtendedException.h"
#include <iostream>
#include "../Application.h"
#include <string>
#include <cassert>
#include "spdlog/spdlog.h"
#include <SDL.h>
#include <SDL_syswm.h>
#include <SDL_platform.h>

//https://github.com/ocornut/imgui/blob/master/examples/example_sdl2_opengl3/main.cpp

void SDLWindowImpl::init_handle()
{
    std::string platform = SDL_GetPlatform();
    auto _window = this->getSdlWindow();

    SDL_SysWMinfo wmi;
    SDL_VERSION(&wmi.version);
    if (!SDL_GetWindowWMInfo(_window, &wmi))
    {
        m_handle = 0;
    }

#	if __LINUX__
#		if ENTRY_CONFIG_USE_WAYLAND
    if (wmi.subsystem == SDL_SYSWM_WAYLAND)
    {
        wl_egl_window* win_impl = (wl_egl_window*)SDL_GetWindowData(_window, "wl_egl_window");
        if (!win_impl)
        {
            int width, height;
            SDL_GetWindowSize(_window, &width, &height);
            struct wl_surface* surface = wmi.info.wl.surface;
            if (!surface)
                m_handle = 0;
            win_impl = wl_egl_window_create(surface, width, height);
            SDL_SetWindowData(_window, "wl_egl_window", win_impl);
        }
        m_handle = (void*)(uintptr_t)win_impl;
    }
    else
#		endif // ENTRY_CONFIG_USE_WAYLAND
        m_handle = (void*)wmi.info.x11.window;
#	elif __MACOSX__ || __IPHONEOS__
    m_handle = wmi.info.cocoa.window;
#	elif __WIN32__
    m_handle = wmi.info.win.window;
#   elif __ANDROID__
    m_handle = wmi.info.android.window;
#	endif // 
}

// SDLWindowImpl constructor
SDLWindowImpl::SDLWindowImpl(Window* window, const std::string& title, int width, int height)
    : WindowImpl(window, title, width, height)
{
    spdlog::info("Launching SDL window implementation");
}

// SDLWindowImpl destructor
SDLWindowImpl::~SDLWindowImpl() {
    // Need to lock Sdl/Glfw to prevent multithreaded access
    auto windowMngLock = lockWindowManagerContext(m_title);
    SDL_DestroyWindow(m_sdl_window);
    SDL_Quit();
}

void SDLWindowImpl::init() {

    spdlog::info("Initializing SDL window implementation");

    // Need to lock Sdl/Glfw to prevent multithreaded access
    {
        auto windowMngLock = lockWindowManagerContext(m_title);

        spdlog::info("Successfully acquired window manager lock in \"" + m_title + "\"");

        // Initialize SDL
        Uint32 SDL_flags = SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER;
        if (!SDL_WasInit(SDL_flags)) {
            if (SDL_Init(SDL_flags) < 0) {
                THROW_EXTENDED_EXCEPTION("Failed to initialize SDL");
            }
        }

        spdlog::info("Completed SDL initialization in \"" + m_title + "\"");
    }

    if (m_renderer_type == RendererType::SDL) {
        initSDLrenderer();
    }
    else if (m_renderer_type == RendererType::OpenGL) {
        initOpenGL();
    }
    else if (m_renderer_type == RendererType::BGFX) {
        initBGFX();
    }
    else if (m_renderer_type == RendererType::LVGL) {
        #ifdef ENABLE_LVGL
        initLVGL();
        #endif
    }
    else if (m_renderer_type == RendererType::none) {
        initEmpty();
    } else {
        THROW_EXTENDED_EXCEPTION("Unrecognized renderer");
    }

    spdlog::info("Initialized renderer in \"" + m_title + "\"");

    // Associate native window with window implementation
    SDL_SetWindowData(m_sdl_window, "p", this);
}

void SDLWindowImpl::initEmpty() {

    // todo: hidden/visible should be set by window caller and apply to all window creation functions
    auto windowMngLock = lockWindowManagerContext(m_title);
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_HIDDEN);
    m_sdl_window = SDL_CreateWindow(m_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_width, m_height, window_flags);
    if (!m_sdl_window) {
        THROW_EXTENDED_EXCEPTION("Failed to create SDL window");
    }

    init_handle(); // populates native window handle
}

void SDLWindowImpl::initSDLrenderer() {

    {
        auto windowMngLock = lockWindowManagerContext(m_title);
        spdlog::info("Creating native window for \"" + m_title + "\"");

        // Create window
        SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_SHOWN);
        m_sdl_window = SDL_CreateWindow(m_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_width, m_height, window_flags);
        if (!m_sdl_window) {
            THROW_EXTENDED_EXCEPTION("Failed to create SDL window");
        }

        init_handle(); // populates native window handle
        spdlog::info("Created native window for \"" + m_title + "\"");

        //spdlog::info("Launching lvgl test \"" + m_title + "\"");
        //lvgltests(m_sdl_window);

        // From 2.0.18: Enable native IME.
#ifdef SDL_HINT_IME_SHOW_UI
        SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
#endif
    }

    // Create renderer
    // Should decide which renderer you want to use here
    m_renderer = std::make_unique<SDLRenderer>(getWindow());
    m_renderer->initialize();
}

void SDLWindowImpl::initOpenGL() {

    auto windowMngLock = lockWindowManagerContext(m_title);
    spdlog::info("Selecting OpenGL options for \"" + m_title + "\"");

    // Already assuming OpenGL at this point
    // Set OpenGL version and profile

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
// GL ES 2.0 + GLSL 100
    std::string glsl_version = "#version 100";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#elif defined(__APPLE__)
// GL 3.2 Core + GLSL 150
    std::string glsl_version = "#version 150";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
// GL 3.0 + GLSL 130
    std::string glsl_version = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif

    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY); // Necessary for deprecated OpenGL functions
    //// SDL_GL_CONTEXT_PROFILE_CORE
    // 
    // From 2.0.18: Enable native IME.
#ifdef SDL_HINT_IME_SHOW_UI
    SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
#endif

    spdlog::info("Creating native window for \"" + m_title + "\"");

    // Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    m_sdl_window = SDL_CreateWindow(m_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_width, m_height, window_flags);
    if (!m_sdl_window) {
        THROW_EXTENDED_EXCEPTION("Failed to create SDL window");
    }

    init_handle(); // populates native window handle
    spdlog::info("Created native window for \"" + m_title + "\"");

    // Create renderer
    // Should decide which renderer you want to use here
    m_renderer = std::make_unique<OpenGLRenderer>(getWindow(), glsl_version);
    m_renderer->makeCurrent();
    //std::unique_lock<std::mutex> contextLock = m_renderer->lockContext();  // Need to lock context
    m_renderer->initialize();
}

void SDLWindowImpl::initBGFX() {
    {
        auto windowMngLock = lockWindowManagerContext(m_title);
        spdlog::info("Creating native window for \"" + m_title + "\"");

        // Create window
        SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_SHOWN);
        m_sdl_window = SDL_CreateWindow(m_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_width, m_height, window_flags);
        if (!m_sdl_window) {
            THROW_EXTENDED_EXCEPTION("Failed to create SDL window");
        }

        init_handle(); // populates native window handle
        spdlog::info("Created native window for \"" + m_title + "\"");

        // From 2.0.18: Enable native IME.
#ifdef SDL_HINT_IME_SHOW_UI
        SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
#endif
    }

    // Create renderer
    // Should decide which renderer you want to use here
    m_renderer = std::make_unique<BGFXRenderer>(getWindow());
    m_renderer->initialize();
}

#ifdef ENABLE_LVGL
void SDLWindowImpl::initLVGL() {
    lvgltests(this->getSdlWindow());
}
#endif

//void SDLWindowImpl::initVulkan() {
//
//    // Create window
//    m_sdl_window = SDL_CreateWindow(m_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_width, m_height, SDL_WINDOW_VULKAN);
//    if (!m_window) {
//        THROW_EXTENDED_EXCEPTION("Failed to create SDL window");
//    }
//
//    // Create renderer
//    // Should decide which renderer you want to use here
//    m_renderer = std::make_unique<OpenGLRenderer>(getWindow());
//    std::unique_lock<std::mutex> contextLock = m_renderer->lockContext();  // Need to lock context
//    m_renderer->initialize();
//}

// Event handling
bool SDLWindowImpl::pollEvent(Event& event) {
    sdlGetEvents();
    return WindowImpl::pollEvent(event);
}

void SDLWindowImpl::sdlGetEvents() { // static for safety
    SDL_Event sdlEvent;
    bool pollret;

    { // Need to lock Sdl/Glfw to prevent multithreaded access
        //auto windowMngLock = lockWindowManagerContext(m_title);
        pollret = SDL_PollEvent(&sdlEvent);
    }

    if (pollret) {
        SDL_Window* window = 0;
        Event event;
        switch (sdlEvent.type) {
        case SDL_QUIT:
            event.type = Event::Type::Quit;
            break;
        case SDL_WINDOWEVENT_RESIZED:
            event.type = Event::Type::WindowResize;
            event.windowResize.width = sdlEvent.window.data1;
            event.windowResize.height = sdlEvent.window.data2;
            break;
        case SDL_MOUSEBUTTONDOWN:
            event.type = Event::Type::MouseButtonDown;
            event.mouseButton.x = sdlEvent.button.x;
            event.mouseButton.y = sdlEvent.button.y;
            event.mouseButton.button = sdlEvent.button.button;
            break;
        case SDL_MOUSEBUTTONUP:
            event.type = Event::Type::MouseButtonUp;
            event.mouseButton.x = sdlEvent.button.x;
            event.mouseButton.y = sdlEvent.button.y;
            event.mouseButton.button = sdlEvent.button.button;
            break;
        case SDL_MOUSEMOTION:
            event.type = Event::Type::MouseMotion;
            event.mouseMotion.x = sdlEvent.motion.x;
            event.mouseMotion.y = sdlEvent.motion.y;
            event.mouseMotion.xrel = sdlEvent.motion.xrel;
            event.mouseMotion.yrel = sdlEvent.motion.yrel;
            break;
        case SDL_MOUSEWHEEL:
            event.type = Event::Type::MouseWheel;
            event.mouseWheel.x = sdlEvent.wheel.x;
            event.mouseWheel.y = sdlEvent.wheel.y;
            break;
        case SDL_KEYDOWN:
            event.type = Event::Type::KeyDown;
            event.key.keycode = sdlEvent.key.keysym.sym;
            event.key.scancode = sdlEvent.key.keysym.scancode;
            event.key.mod = sdlEvent.key.keysym.mod;
            break;
        case SDL_KEYUP:
            event.type = Event::Type::KeyUp;
            event.key.keycode = sdlEvent.key.keysym.sym;
            event.key.scancode = sdlEvent.key.keysym.scancode;
            event.key.mod = sdlEvent.key.keysym.mod;
            break;
        case SDL_TEXTINPUT:
            event.type = Event::Type::TextInput;
            strncpy(event.textInput.text, sdlEvent.text.text, sizeof(event.textInput.text) - 1);
            event.textInput.text[sizeof(event.textInput.text) - 1] = '\0';
            break;
        default:
            event.type = Event::Type::None;
        }

        if (window) {
            getCallingWindow(window)->eventQueue.push(event);
        }
        else {
            app.getImpl()->addEvent(event);
        }
    }
}

// Getters

SDL_Window* SDLWindowImpl::getSdlWindow() const {
    assert(m_sdl_window);
    return m_sdl_window;
}

//void* SDLWindowImpl::getNativeHandle() const {
//
//    SDL_GetPlatform();
//
//    SDL_SysWMinfo wmi;
//    SDL_VERSION(&wmi.version);
//    if (!SDL_GetWindowWMInfo(m_sdl_window, &wmi))
//    {
//        return NULL;
//    }
//
//    #if ( __LINUX__ || __FREEBSD__ || __BSDI__ || __NETBSD__ || __OpenBSD__)
//        if (wmi.subsystem == SDL_SYSWM_WAYLAND) {
//            wl_egl_window* win_impl = (wl_egl_window*)SDL_GetWindowData(_window, "wl_egl_window");
//            if (!win_impl)
//            {
//                int width, height;
//                SDL_GetWindowSize(_window, &width, &height);
//                struct wl_surface* surface = wmi.info.wl.surface;
//                if (!surface)
//                    return nullptr;
//                win_impl = wl_egl_window_create(surface, width, height);
//                SDL_SetWindowData(_window, "wl_egl_window", win_impl);
//            }
//            return (void*)(uintptr_t)win_impl;
//        }
//        else if (wmi.subsystem == SDL_SYSWM_X11) {
//            return (void*)wmi.info.x11.window;
//        }
//        else {
//            THROW_EXTENDED_EXCEPTION("Unknown *nix window type");
//        }
//    #elif (__MACOSX__)
//        if (wmi.subsystem != SDL_SYSWM_COCOA) THROW_EXTENDED_EXCEPTION("Window type mismatch");
//        return wmi.info.cocoa.window;
//    #elif (__WINDOWS__)
//        if (wmi.subsystem != SDL_SYSWM_WINDOWS) THROW_EXTENDED_EXCEPTION("Window type mismatch");
//        return wmi.info.win.window;
//    #elif (__ANDROID__) // See Flare engine for an example of a working SDL2 Android implementation: https://github.com/flareteam/flare-engine/wiki/Android-Port
//        if (wmi.subsystem != SDL_SYSWM_ANDROID) THROW_EXTENDED_EXCEPTION("Window type mismatch");
//        return wmi.info.android.window;     
//    #elif (__IPHONEOS__ || __TVOS__)
//        if (wmi.subsystem != SDL_SYSWM_UIKIT) THROW_EXTENDED_EXCEPTION("Window type mismatch");
//        return wmi.info.uikit.window;
//    #elif (__WINRT__) //SDL_SYSWM_WINRT
//        if (wmi.subsystem != SDL_SYSWM_WINRT) THROW_EXTENDED_EXCEPTION("Window type mismatch");
//        return wmi.info.winrt.window;
//    #else
//        THROW_EXTENDED_EXCEPTION("Unknown/unsupported platform");
//    #endif // Platform selection
//
//    // Add Vivante
//}

//WindowingSystem SDLWindowImpl::getWindowingSystem() const {
//    return WindowingSystem::SDL;
//}

SDLWindowImpl* SDLWindowImpl::getCallingWindow(SDL_Window* window) {
    return (SDLWindowImpl *)SDL_GetWindowData(window, "p");
}

// Setters
void SDLWindowImpl::setTitleUnsafe(char* title) {
    SDL_SetWindowTitle(m_sdl_window, title);
}

void SDLWindowImpl::setSizeUnsafe(int width, int height) {
    SDL_SetWindowSize(m_sdl_window, width, height);
}