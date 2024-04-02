#pragma once

#include <SDL.h>
#include "WindowImpl.h"

class Window;

class SDLWindowImpl : public WindowImpl {
public:
    // Constructor
    SDLWindowImpl(Window* window, const std::string& title, int width, int height);

    // Destructor
    ~SDLWindowImpl();

    // Event handling
    bool pollEvent(Event& event) override;
    void sdlGetEvents();

    // Getters
    SDL_Window* getSdlWindow() const override;
    //void* getNativeHandle() const override;
    //WindowingSystem getWindowingSystem() const override;
    static SDLWindowImpl* getCallingWindow(SDL_Window* window);
protected:
    void init() override;
    void initSDLrenderer();
    void initOpenGL();
    void initBGFX();
    void initEmpty();
    #ifdef ENABLE_LVGL
    void initLVGL();
    #endif
    void init_handle() override;

    //void initVulkan();
    //void initMetal();
    //void initDirectX();

    // Setters
    void setTitleUnsafe(char* title) override;
    void setSizeUnsafe(int width, int height) override;

private:
    SDL_Window* m_sdl_window = 0;
};