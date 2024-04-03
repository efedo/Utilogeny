#include "BGFXRenderer.h"
#include "BGFXRendererImpl.h"
#include <bx/bx.h>
#include <bx/math.h>

BGFXRenderer::BGFXRenderer(Window* window)
    : Renderer(window)
{
    // Create internal bgfx renderer object
    m_bgfx_internal = new BgfxRendererImpl(this);
}

//Todo: make thread safe (also make constructors thread safe)
BGFXRenderer::~BGFXRenderer() {

    if (m_bgfx_internal) delete m_bgfx_internal;
    m_bgfx_internal = 0;
}

void BGFXRenderer::initialize() {
    m_bgfx_internal->initialize();
}

void BGFXRenderer::imguiInit() {
    m_bgfx_internal->imguiInit();
}

void BGFXRenderer::clear() {
    // This function is not needed for bgfx since the clearing is done in the render() function
}

void BGFXRenderer::render() {
    m_bgfx_internal->render();
}

void BGFXRenderer::renderImgui() {
    m_bgfx_internal->renderImgui();
}

void BGFXRenderer::swapBuffers() {
    m_bgfx_internal->swapBuffers();
}

void BGFXRenderer::setBackColor(RGBA8 color) {
    m_bgfx_internal->setBackColor(color);
}

void BGFXRenderer::makeCurrent() {
    // not yet implemented
}

void BGFXRenderer::drawRect(Rect rect, RGBA8 color) {
    // not yet implemented
}