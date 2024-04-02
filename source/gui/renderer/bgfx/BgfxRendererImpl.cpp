/*
 * Copyright 2011-2023 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx/blob/master/LICENSE
 */

 // BGFXRenderer.cpp
 //#include <GL/glew.h>

#include "source/window/Window.h"

#include "BGFXRenderer.h"
#include "BGFXRendererImpl.h"
#include "BGFXRendererContext.h"

#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bx/bx.h>
#include <bx/math.h>

#include "source/renderer/ImGUIManager.h"
#include "imgui.h"
#include "imgui/imgui_impl_bgfx.h"

#include <SDL.h>
//#include <SDL_syswm.h>
//#include <SDL_platform.h>
//#include "backends/imgui_impl_sdl2.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#if GLFW_VERSION_MINOR < 2
#	error "GLFW 3.2 or later is required"
#endif // GLFW_VERSION_MINOR < 2

#if BX_PLATFORM_LINUX
#	if ENTRY_CONFIG_USE_WAYLAND
#		include <wayland-egl.h>
#		define GLFW_EXPOSE_NATIVE_WAYLAND
#	else
#		define GLFW_EXPOSE_NATIVE_X11
#		define GLFW_EXPOSE_NATIVE_GLX
#	endif
#elif BX_PLATFORM_OSX
#	define GLFW_EXPOSE_NATIVE_COCOA
#	define GLFW_EXPOSE_NATIVE_NSGL
#elif BX_PLATFORM_WINDOWS
#	define GLFW_EXPOSE_NATIVE_WIN32
#	define GLFW_EXPOSE_NATIVE_WGL
#endif //
#include <GLFW/glfw3native.h>

#include "backends/imgui_impl_glfw.h"

#include "source/window/Window.h"
#include <iostream>
#include "source/ExtendedException.h"
#include "spdlog/spdlog.h"

// https://github.com/bkaradzic/bgfx/blob/master/examples/22-windows/windows.cpp

BgfxRendererContext* BgfxRendererImpl::m_context = 0;

BgfxRendererImpl::BgfxRendererImpl(BGFXRenderer* bgfx_renderer)
    : m_renderer(bgfx_renderer), m_handle(bgfx_renderer->getWindow()->getNativeHandle())
{
    auto lock = m_renderer->lockContext();

    if (!m_context) m_context = new BgfxRendererContext();
    m_context->m_numThreads++;
    
    // Todo: check max views supported by bgfx
    //size_t i = 1;
    size_t i = 0;
    for (; i < 100; i++) { // 0 is reserved
        if (m_context->m_used_views[i] == false) {
            m_context->m_used_views[i] = true;
            m_view = i;
            break;
        }
    }

    if (i == 100) spdlog::critical("Too many windows already created");

    m_fbh.idx = bgfx::kInvalidHandle;
}

BgfxRendererImpl::~BgfxRendererImpl() {

    auto lock = m_renderer->lockContext();

    if (bgfx::isValid(m_fbh))
    {
        bgfx::destroy(m_fbh);
        m_fbh.idx = bgfx::kInvalidHandle;

        if (m_view) bgfx::setViewFrameBuffer(m_view, { bgfx::kInvalidHandle });

        // Flush destruction of swap chain before destroying window!
        //bgfx::frame();
        //bgfx::frame();
        m_context->m_queue.put(PolyM::Msg(int(BgfxContextSignal::frame)));
        m_context->m_queue.put(PolyM::Msg(int(BgfxContextSignal::frame)));
    }

    if (m_view) m_context->m_used_views[m_view] = false;

    if (!m_context) return;

    m_context->m_numThreads--;

    // Shutdown bgfx if last thread
    if (m_context->m_numThreads <= 1) {
        m_context->bgfx_init = 0;
        bgfx::destroy(m_context->m_ibh);
        bgfx::destroy(m_context->m_vbh);
        bgfx::destroy(m_context->m_program);
        bgfx::shutdown();
        delete m_context;
        m_context = 0;
    }
}

void BgfxRendererImpl::initialize() {

    // Initialize global (not window-specific) bgfx context
    {
        auto lock = m_renderer->lockContext();

        if (!m_context) spdlog::critical("Missing bgfx global context object!");
        if (!m_context->bgfx_init) {
            m_context->m_queue.request(PolyM::Msg(int(BgfxContextSignal::init)));
            bgfx::resetView(0); // clear the default fbh from the primary window
                                // note: leaks a framebuffer? No way to obtain default framebuffer handle
                                // in user land?
        }
        else {
            //m_primary_window = false;
            spdlog::info("BGFX already initialized, using same context for all windows");
        }

        if (m_context->m_numThreads == m_context->m_maxThreads) {
            spdlog::critical("Exceeding maximum number of Bgfx threads/windows!");
        }
    }

    //bgfx::Encoder* encoder = bgfx::begin();

    //// Set up framebuffer
    ////bx::memSet(m_fbh, 0xff, sizeof(m_fbh));

    // Initialize ImGui for bgfx
    imguiInit();

    //bgfx::end(encoder);

    //m_context->m_queue.put(PolyM::Msg(int(BgfxContextSignal::frame)));

    spdlog::info("BGFX renderer creation completed");
}

void BgfxRendererImpl::imguiInit() {

    spdlog::info("Initializing ImGUI for Bgfx");

    m_renderer->m_imguiContextMngr = new ImGuiContextManager;
    auto imguiLock = m_renderer->m_imguiContextMngr->lockImgui();

    ImGui_Implbgfx_Init(m_view);

    //// Setup Platform/Renderer backends
    //if (WindowSDL()) {
    //    ImGui_ImplSDL2_InitForOpenGL(getSdlWindow(), m_glContext);
    //    ImGui_ImplOpenGL3_Init(m_glsl_version.c_str());
    //}
    //else if (WindowGLFW()) {
    //    GLFWwindow* win = getGlfwWindow();
    //    ImGui_ImplGlfw_InitForOpenGL(win, false);
    //    ImGui_ImplOpenGL3_Init(m_glsl_version.c_str());
    //}

    m_renderer->m_imguiContextMngr->loadFonts();

    spdlog::info("Finished initializing ImGUI for Bgfx");
}

void BgfxRendererImpl::render() {
    // see https://github.com/bkaradzic/bgfx/blob/master/examples/17-drawstress/drawstress.cpp
    // and https://github.com/bkaradzic/bgfx/blob/master/examples/22-windows/windows.cpp

    auto lock = m_renderer->lockContext();

    //if (win.m_nwh != m_state.m_nwh || (win.m_width != m_state.m_width || win.m_height != m_state.m_height))
    
    //todo: fix glfw window to actually update width and height
    uint16_t newwidth = m_renderer->getWindow()->getWidth();
    uint16_t newheight = m_renderer->getWindow()->getHeight();
   
    if ((m_width != newwidth) || (m_height != newheight))
    {
        m_width = newwidth;
        m_height = newheight;
        
        //if (!m_primary_window) {
            // When window changes size or native window handle changed
            // frame buffer must be recreated.
            if (bgfx::isValid(m_fbh))
            {
                bgfx::destroy(m_fbh); // not sure if this can be called from outside main thread?
                m_fbh.idx = bgfx::kInvalidHandle;
            }

            if (NULL != this->m_handle)
            {
                // bgfx::createFrameBuffer is not thread-safe, at least with DX11?
                m_fbh = bgfx::createFrameBuffer(this->m_handle, m_width, m_height);

                //BgfxFBSetup fb_setup;
                //fb_setup.handle = this->m_handle;
                //fb_setup.width = m_width;
                //fb_setup.height = m_height;
                //fb_setup.fbh = &m_fbh;
                //m_context->m_queue.request(PolyM::DataMsg<BgfxFBSetup>(int(BgfxContextSignal::setup_fb), fb_setup));
            }

            if (bgfx::isValid(m_fbh)) {
                bgfx::setViewFrameBuffer(m_view, m_fbh);
            }
        //}

        // Set up the view
        //if (m_primary_window || bgfx::isValid(m_fbh)) {
        if (bgfx::isValid(m_fbh)) {
            bgfx::setViewName(m_view, m_renderer->getWindow()->getTitle().c_str());
            setViewClear();
            bgfx::setViewRect(m_view, 0, 0, m_width, m_height);
        }
    }

    bgfx::Encoder* encoder = bgfx::begin();
    if (encoder) { // when you're not actually rendering, sometimes fails to get lock on encoder
        // Submit the clear operation to bgfx
        encoder->touch(m_view);
        bgfx::end(encoder);
        //m_context->m_queue.request(PolyM::Msg(int(BgfxContextSignal::frame)));
    }

    renderImgui();

    m_context->m_queue.request(PolyM::Msg(int(BgfxContextSignal::frame)));
}

void BgfxRendererImpl::renderImgui() {
    auto imguiLock = m_renderer->m_imguiContextMngr->lockImgui();

    // For some reason this crashes

    //// ImGui new frame
    //ImGui_Implbgfx_NewFrame(m_renderer->getWindow()->getWidth(), m_renderer->getWindow()->getHeight());

    //// ImGui rendering
    //ImGui::NewFrame();

    ////m_imguiContextMngr->testDraw();

    //ImGui::Render();
    //ImGui_Implbgfx_RenderDrawLists(ImGui::GetDrawData());
}

void BgfxRendererImpl::swapBuffers() {
    // bgfx handles buffer swapping automatically when frame() is called
    //bgfx::frame();
}

void BgfxRendererImpl::setViewClear() {
    if (bgfx::isValid(m_fbh)) {
        bgfx::setViewClear(m_view, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, m_renderer->getBackColor(), 1.0f, 0);
    }
}

void BgfxRendererImpl::setBackColor(RGBA8 color) {
    m_renderer->Renderer::setBackColor(color);
    setViewClear();
}
