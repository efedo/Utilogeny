#include "WindowImpl.h"
#include "../event/Event.h"
#include <cassert>
#include "spdlog/spdlog.h"
#include "../ExtendedException.h"

std::mutex WindowImpl::WndManagerMutex;

// Constructor
WindowImpl::WindowImpl(Window* window, const std::string& title, int width, int height)
    : m_parentWindow(window), m_title(title), m_width(width), m_height(height)
{
    assert(m_parentWindow);
}

// Destructor
WindowImpl::~WindowImpl() {
}

SDL_Window* WindowImpl::getSdlWindow() const {
    THROW_EXTENDED_EXCEPTION("Tried to get an SDL Window pointer for a non-SDL Window.");
}

GLFWwindow* WindowImpl::getGlfwWindow() const {
    THROW_EXTENDED_EXCEPTION("Tried to get a GLFW Window pointer for a non-GLFW Window.");
}

void* WindowImpl::getNativeHandle() const {
    return this->m_handle;
}

Window* WindowImpl::getWindow() const {
    return m_parentWindow;
}

// Rendering control
void WindowImpl::render() {
    if (!m_renderer) return;
    m_renderer->makeCurrent();
    //std::unique_lock<std::mutex> contextLock = m_renderer->lockContext();  // Need to lock context
    m_renderer->clear();
    m_renderer->drawRect(Rect{ 100, 100, 100, 100 }, RGBA8{ 0,0,255,255 });
    m_renderer->render();
    m_renderer->swapBuffers();
}

void WindowImpl::setTitle(const std::string& title) {
    m_title = title;
    Event event;
    event.type = Event::Type::CmdSetWindowTitle;
    std::string::size_type size = m_title.size();
    if (size > 255) size = 255; 
    memcpy(event.setWindowTitle.title, m_title.c_str(), size + 1); // we need extra char for NUL
    addEvent(event);
}

void WindowImpl::setSize(int width, int height) {
    m_width = width;
    m_height = height;
    Event event;
    event.type = Event::Type::CmdSetWindowSize;
    event.setWindowSize.width = m_width;
    event.setWindowSize.height = m_height;
    addEvent(event);
}

void WindowImpl::setBackColor(RGBA8 color) {
    m_renderer->setBackColor(color);
}

// Called by Window thread only
void WindowImpl::processEvents() {
    Event event;
    while (pollEvent(event)) {
        if (event.type == Event::Type::CmdSetWindowTitle) {
            setTitleUnsafe(event.setWindowTitle.title);
        }
        else if (event.type == Event::Type::CmdSetWindowSize) {
            setSizeUnsafe(event.setWindowSize.width, event.setWindowSize.height);
        }
    };
}

std::unique_lock<std::mutex> WindowImpl::lockWindowManagerContext(std::string caller) {
    // Lock the mutex to synchronize access to the OpenGL context
    //spdlog::info("Locking window manager mutex from: \"" + caller + "\"");
    std::unique_lock<std::mutex> lock(WndManagerMutex);
    return std::move(lock);
}

void WindowImpl::init_handle() {
    THROW_EXTENDED_EXCEPTION("Should have been overridden.");
}

//void WindowImpl::pushCommand(Command command) {
//
//}
//
//bool WindowImpl::pollCommand(Command& command) {
//    if (!commandQueue.empty()) {
//        command = commandQueue.front();
//        commandQueue.pop();
//        return true;
//    }
//
//    command.m_type = Command::Type::None;
//    return false;
//}