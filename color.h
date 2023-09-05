//
// Created by david on 8/26/23.
//

#ifndef RAYTRACER_COLOR_H
#define RAYTRACER_COLOR_H

#include "vec3.h"

#include <iostream>

using color = vec3;

void write_color(std::ostream &out, color pixel_color) {
    // Write the translated [0,255] value of each color component [0, 1].
    out << static_cast<int>(255.999 * pixel_color.x()) << ' '
        << static_cast<int>(255.999 * pixel_color.y()) << ' '
        << static_cast<int>(255.999 * pixel_color.z()) << '\n';
}

void write_color_antialias(std::ostream &out, color pixel_color, int samples_per_pixel) {
    static const interval intensity(0.000, 0.999);

    double scale = 1.0 / samples_per_pixel;
    double r = intensity.clamp(pixel_color.x() * scale);
    double g = intensity.clamp(pixel_color.y() * scale);
    double b = intensity.clamp(pixel_color.z() * scale);

    // Write the translated [0,255] value of each color component [0, 1].
    out << static_cast<int>(255.999 * r) << ' '
        << static_cast<int>(255.999 * g) << ' '
        << static_cast<int>(255.999 * b) << '\n';
}

#endif //RAYTRACER_COLOR_H
