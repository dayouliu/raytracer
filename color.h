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

#endif //RAYTRACER_COLOR_H
