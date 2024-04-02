#include "Color.h"

RGBA8::RGBA8(const RGBAf& color) {
    r = static_cast<uint8_t>(color.r * 255.0f + 0.5f);
    g = static_cast<uint8_t>(color.g * 255.0f + 0.5f);
    b = static_cast<uint8_t>(color.b * 255.0f + 0.5f);
    a = static_cast<uint8_t>(color.a * 255.0f + 0.5f);
}

RGBA8::operator uint32_t() const {
    uint32_t result =
        (uint32_t(r) << 24) +
        (uint32_t(g) << 16) +
        (uint32_t(b) << 8) +
        (uint32_t(a));
    return result;
}

RGBAf::RGBAf(const RGBA8& color) {
    r = static_cast<float>(color.r) / 255.0f;
    g = static_cast<float>(color.g) / 255.0f;
    b = static_cast<float>(color.b) / 255.0f;
    a = static_cast<float>(color.a) / 255.0f;
}