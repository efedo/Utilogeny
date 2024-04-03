// SDLRenderer.h

#pragma once

#include <SDL.h>
#include "Utilogeny/source/gui/lib/renderer/Renderer.h"
#include "Utilogeny/source/gui/lib/window/WindowImpl.h"

class SDLRenderer : public Renderer {
public:
    SDLRenderer(Window* window);
    virtual ~SDLRenderer();

    void initialize() override;
    void imguiInit() override;
    void clear() override;
    void render() override;
    void renderImgui() override;
    void swapBuffers() override;
    void makeCurrent() override;
    void drawRect(Rect rect, RGBA8 color) override;
private:
    void setDrawColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    void fillRect(const Rect& rect);
    SDL_Renderer* m_renderer;
};
