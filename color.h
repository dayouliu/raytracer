#pragma once

#include <iostream>
#include "vec3.h"

using color = vec3;

inline double linear_to_gamma(double linear) {
    return std::sqrt(linear);
}

void write_color(std::ostream &out, color pixel_color);

void write_color_antialias(std::ostream &out, color pixel_color, int samples_per_pixel);