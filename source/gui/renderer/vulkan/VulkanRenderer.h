// SDLRenderer.h

#pragma once

#include "Renderer.h"
#include "../window/Window.h"

struct VulkanStateStruct;

class VulkanRenderer : public Renderer {
public:
    VulkanRenderer(Window* window);
    virtual ~VulkanRenderer();

    void initialize() override;
    void initializeSDL();
    void initializeGLFW();
    void clear() override;
    void render() override;
    void swapBuffers() override;
    void makeCurrent() override;

    void drawRect(Rect rect, RGBA8 color) override;
    void setDrawColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    void fillRect(const Rect& rect);

private:
    VulkanStateStruct* vk_state;
};
