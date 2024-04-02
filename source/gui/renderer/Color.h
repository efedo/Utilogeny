#pragma once

#include <stdint.h>

struct Color {};

struct RGBAf;
struct RGBA8;

struct RGBA8 : public Color {
	RGBA8(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a) : r(_r), g(_g), b(_b), a(_a) {}
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
	RGBA8(const RGBAf& color);
	operator uint32_t() const;
};

struct RGBAf : public Color {
	RGBAf(float _r, float _g, float _b, float _a) : r(_r), g(_g), b(_b), a(_a) {}
	float r;
	float g;
	float b;
	float a;
	RGBAf(const RGBA8& color);
};

