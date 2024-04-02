#include "widget.h"

void Button::handleEvent(const Event& event) {
    if (event.type == Event::Type::MouseButtonDown) {
        int x = event.mouseButton.x, y = event.mouseButton.y;
        if (isInside(x, y)) {
            onClick();
        }
    }
}

void Button::render() {
    // Render button background
    //renderer.drawRect(rect, RGBA8{ 255,0,0,255 });

    // Render button text
    // Add code to render the text using your preferred font rendering library
}

bool Button::isInside(int x, int y) {
    return x >= rect.x && x < rect.x + rect.w && y >= rect.y && y < rect.y + rect.h;
}
