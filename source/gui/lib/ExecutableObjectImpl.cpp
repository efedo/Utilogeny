#include "ExecutableObjectImpl.h"
#include "settings.h"

ExecutableObjectImpl::~ExecutableObjectImpl() {
    destructor_called = true;
    stop();
    if (m_loop) m_loop->join();
    m_loop.reset();
}

// redirects to virtual execloop when creating new thread
void ExecutableObjectImpl::execloop_thread_redir() {
    execloop();
}

void ExecutableObjectImpl::exec() {
    if (destructor_called) return; // if destructor called
    stopping_exec = false; // resume execution if destructor not called
    if (m_loop) return; // if exec thread already created

    m_loop.reset(new std::thread(&ExecutableObjectImpl::execloop_thread_redir, this));

    // exec() should pause here until initialization in exec thread has completed
    m_init_future.wait(); // Wait for m_impl to be created
}

void ExecutableObjectImpl::execloop() {

    // Needs to be called in same thread as render/event loop
    // but also needs to halt parent thread execution until fully created (otherwise
    // get messy SDL collisions if creating multiple windows)
    init();

    // should allow exec() to proceed once you've reached here
    m_init_promise.set_value(); // Signal that m_impl has been created

    // Calculate the minimum time between frames
    const std::chrono::duration<double> minFrameTime(1.0 / settings.maxRefreshRate);

    while (!destructor_called) {
        auto frameStartTime = std::chrono::steady_clock::now();

        if (!stopping_exec) {
            processEvents(); // Event handling
            render();
        }

        auto frameEndTime = std::chrono::steady_clock::now();
        auto frameDuration = frameEndTime - frameStartTime;

        // Sleep for the remaining time if the frame was processed faster than the minimum frame time
        // Eventually should add a bit of margin for error here
        // Maybe synchronize properly
        if (frameDuration < minFrameTime) {
            std::this_thread::sleep_for(minFrameTime - frameDuration);
        }
    }
}

void ExecutableObjectImpl::stop() {
    stopping_exec = true;
}

void ExecutableObjectImpl::addEvent(Event event) {
    eventQueue.push(event);
}

void ExecutableObjectImpl::init() {
    // nothing to initialize in base class
}

void ExecutableObjectImpl::render() {
    // Nothing to render unless specialized
}

// Event handling
bool ExecutableObjectImpl::pollEvent(Event& event) {
    // Note: specialized virtual member functions take care of getting events 
    // from SDL/GLFW prior to calling base pollEvent here

    if (!eventQueue.empty()) {
        event = eventQueue.front();
        eventQueue.pop();
        return true;
    }

    event.type = Event::Type::None;
    return false;
}

void ExecutableObjectImpl::processEvents() {
    Event event;
    while (pollEvent(event)) {



    };
}