#pragma once

#include <thread>
#include <future>
#include <queue>
#include "event/event.h"

class ExecutableObjectImpl {
public:
	~ExecutableObjectImpl();
	void exec();
	void stop();
	void addEvent(Event event);

protected:
	virtual void init();
	virtual void render();
	virtual void processEvents();
	virtual bool pollEvent(Event& event);

	bool stopping_exec = false;
	bool destructor_called = false;

	// Main loop thread
	void execloop_thread_redir(); // redirects to virtual execloop when creating new thread
	virtual void execloop(); // Needs to be in the same thread as event polling on Windows
	std::unique_ptr<std::thread> m_loop = 0;

	// To ensure successful initiatilization in execloop thread before continuing in main thread
	std::promise<void> m_init_promise;
	std::future<void> m_init_future = m_init_promise.get_future();

	// Must implement a separate event queue (even for SDL) to guarantee that all events 
	// will be processed by the correct window execloop thread
	// Also allows abstraction of underlying windowing libraries
	std::queue<Event> eventQueue;
};