#pragma once

#include <string>
#include <functional>
#include "../renderer/Renderer.h"
#include "../event/Event.h"

class Widget {
public:
    Widget(Renderer& renderer, int x, int y, int width, int height)
        : renderer{ renderer }, rect{ x, y, width, height } {}

    virtual ~Widget() = default;

    virtual void handleEvent(const Event& event) = 0;
    virtual void render() = 0;

protected:
    Renderer& renderer;
    Rect rect;
};

class Button : public Widget {
public:
    Button(Renderer& renderer, int x, int y, int width, int height, const std::string& text, std::function<void()> onClick)
        : Widget(renderer, x, y, width, height), text{ text }, onClick{ onClick } {}

    void handleEvent(const Event& event) override;
    void render() override;

private:
    bool isInside(int x, int y);

    std::string text;
    std::function<void()> onClick;
};
