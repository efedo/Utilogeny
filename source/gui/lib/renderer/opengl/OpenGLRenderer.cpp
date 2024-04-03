// OpenGLRenderer.cpp

#include <GL/glew.h>
#include "OpenGLRenderer.h"
#include "Utilogeny/source/gui/lib/renderer/ImGUIManager.h"

#include <SDL_opengl.h>
#include <SDL.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "Utilogeny/source/gui/lib/window/WindowImpl.h"
#include <iostream>
#include "Utilogeny/source/gui/lib/ExtendedException.h"
#include "spdlog/spdlog.h"

// todo: make thread safe: lock m_contextMutex for all opengl calls
// todo: also make thread-safe wrt qt drawing? may need to tie update loop to qt render loop
// todo: remember to update qt container widgets to incorporate all fixes from abstractorizer (e.g. disable draw)

OpenGLRenderer::OpenGLRenderer(Window* window, std::string glsl_version)
    : Renderer(window), m_glsl_version(glsl_version)
{
    assert(window);
}

OpenGLRenderer::~OpenGLRenderer() {
    if ((WindowSDL()) && (m_glContext != 0)) {
        SDL_GL_DeleteContext(m_glContext);
    }
}

void OpenGLRenderer::initialize() {
    spdlog::info("Creating OpenGl renderer");

    // SDL needs GL context to be created manually
    if (WindowSDL()) {

        // Create GL context
        SDL_Window* win = getSdlWindow();
        m_glContext = SDL_GL_CreateContext(win);
        if (!m_glContext) {
            THROW_EXTENDED_EXCEPTION("Failed to create SDL GL context");
        }

        // Enable vsync
        SDL_GL_SetSwapInterval(1);
    }

    // Initialize GLEW and configure OpenGL settings
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        // Handle GLEW initialization error
    }

    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    setBackColor(RGBAf{ 0.1f, 0.5f, 0.1f, 0.5f });
    //glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);

    //glViewport(0, 0, 800, 600);  // Set the viewport
    glMatrixMode(GL_PROJECTION);  // Set the projection matrix
    glLoadIdentity();
    glOrtho(0.0f, 800.0f, 0.0f, 600.0f, -1.0f, 1.0f);  // Set the orthographic projection
    glMatrixMode(GL_MODELVIEW);  // Set the modelview matrix
    glLoadIdentity();

    imguiInit();

    spdlog::info("OpenGl renderer creation completed");
}

void OpenGLRenderer::imguiInit() {

    spdlog::info("Initializing ImGUI for OpenGL");

    m_imguiContextMngr = new ImGuiContextManager;
    auto imguiLock = m_imguiContextMngr->lockImgui();

    // Setup Platform/Renderer backends
    if (WindowSDL()) {
        ImGui_ImplSDL2_InitForOpenGL(getSdlWindow(), m_glContext);
        ImGui_ImplOpenGL3_Init(m_glsl_version.c_str());
    }
    else if (WindowGLFW()) {
        GLFWwindow * win = getGlfwWindow();
        ImGui_ImplGlfw_InitForOpenGL(win, false);
        ImGui_ImplOpenGL3_Init(m_glsl_version.c_str());
    }

    m_imguiContextMngr->loadFonts();

    spdlog::info("Finished initializing ImGUI for OpenGL");
}

void OpenGLRenderer::clear() {
    RGBAf fltcolor = backColor;
    glClearColor(fltcolor.r, fltcolor.g, fltcolor.b, fltcolor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRenderer::render() {
    glFlush();  // Flush the buffer
    renderImgui();
}

void OpenGLRenderer::renderImgui() {
    auto lock = m_imguiContextMngr->lockImgui();

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();

    if (WindowSDL()) {
        ImGui_ImplSDL2_NewFrame();
    }
    else if (WindowGLFW()) {
        ImGui_ImplGlfw_NewFrame();
    }

    m_imguiContextMngr->testDraw();

    ImGui::Render();
    glViewport(0, 0, m_imguiContextMngr->getDisplaySizeX(), m_imguiContextMngr->getDisplaySizeY());
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void OpenGLRenderer::swapBuffers() {
    // May need to make current first

    if (WindowSDL()) {
        SDL_GL_SwapWindow(getSdlWindow());
    }
    else if (WindowGLFW()) {
        glfwSwapBuffers(getGlfwWindow());
    }
    else {
        std::cerr << "Unknown window type" << std::endl;
    }
}

void OpenGLRenderer::makeCurrent() {
    void* window = this->m_window;
    if (window == m_currentWindow) return;

    if (WindowSDL()) {
        if (SDL_GL_MakeCurrent(getSdlWindow(), m_glContext) != 0) {
            // Handle context setting error
        }
    }
    else if (WindowGLFW()) {
        glfwMakeContextCurrent(getGlfwWindow());
    }
    else {
        std::cerr << "Unknown window type" << std::endl;
    }

    m_currentWindow = window;
}

void OpenGLRenderer::drawRect(Rect rect, RGBA8 color) {
    setDrawColor(color.r, color.g, color.b, color.a);
    fillRect(rect);
}

void OpenGLRenderer::setDrawColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    glColor4ub(r, g, b, a);
}

void OpenGLRenderer::fillRect(const Rect& rect) {
    glBegin(GL_QUADS);
    glVertex2i(rect.x, rect.y);
    glVertex2i(rect.x + rect.w, rect.y);
    glVertex2i(rect.x + rect.w, rect.y + rect.h);
    glVertex2i(rect.x, rect.y + rect.h);
    glEnd();
}