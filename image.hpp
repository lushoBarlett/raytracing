#pragma once

#include <iostream>

#include "vec3.hpp"

inline uint8_t color_clamp(int x) {
	if (x < 0)
		return 0;
	
	if (x > 255)
		return 255;
	
	return x;
}

void write_color(const std::ostream& out, const vec3& color, int samples) {
	uint8_t r = color_clamp(255 * std::sqrt(color.x / samples));
	uint8_t g = color_clamp(255 * std::sqrt(color.y / samples));
	uint8_t b = color_clamp(255 * std::sqrt(color.z / samples));

	std::cout << r << g << b;
}
