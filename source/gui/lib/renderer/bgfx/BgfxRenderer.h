// BGFXRenderer.h
#pragma once

// https://github.com/bkaradzic/bgfx/blob/master/examples/22-windows/windows.cpp

#include "Utilogeny/source/gui/lib/renderer/Renderer.h"

class BgfxRendererImpl;

// Probably have to make a single main Bgfx caller thread

class BGFXRenderer : public Renderer {
public:
    BGFXRenderer(Window * window);
    ~BGFXRenderer();
    virtual void initialize() override;
    virtual void imguiInit() override;
    virtual void clear() override;
    virtual void render() override;
    virtual void renderImgui() override;
    virtual void swapBuffers() override;
    virtual void setBackColor(RGBA8 color) override;
    virtual void makeCurrent() override;
    void drawRect(Rect rect, RGBA8 color) override;
private:
    friend class BgfxRendererImpl;
    BgfxRendererImpl* m_bgfx_internal = 0;
};
