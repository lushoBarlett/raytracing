#pragma once

#include <iostream>

#include "vec3.hpp"

inline int color_clamp(int x) {
	if (x < 0)
		return 0;
	
	if (x > 255)
		return 255;
	
	return x;
}

void write_color(const std::ostream& out, const vec3& color, int samples) {
	int r = color_clamp(255 * std::sqrt(color.x / samples));
	int g = color_clamp(255 * std::sqrt(color.y / samples));
	int b = color_clamp(255 * std::sqrt(color.z / samples));

	std::cout << r << ' ' << g << ' ' << b << '\n';
}
