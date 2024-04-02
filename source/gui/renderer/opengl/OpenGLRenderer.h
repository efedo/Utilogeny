// OpenGLRenderer.h
#pragma once

#include "source/renderer/Renderer.h"

class OpenGLRenderer : public Renderer {
public:
    OpenGLRenderer(Window* window, std::string glsl_version);
    ~OpenGLRenderer();
    virtual void initialize() override;
    virtual void imguiInit() override;
    virtual void clear() override;
    virtual void render() override;
    virtual void renderImgui() override;
    virtual void swapBuffers() override;
    virtual void makeCurrent() override;
    void drawRect(Rect rect, RGBA8 color) override;
private:
    void setDrawColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    void fillRect(const Rect& rect);
//  virtual void getMouseState(int& x, int& y) override;
    void * m_glContext = 0;
    const std::string m_glsl_version;
};

