//
// Created by david on 9/3/23.
//

#ifndef RAYTRACER_RTMATH_H
#define RAYTRACER_RTMATH_H

// Common headers
#include <cmath>
#include <limits>
#include <memory>

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

const double INF = std::numeric_limits<double>::infinity();
const double PI = M_PI;

inline double deg_to_rad(double degrees) {
    return degrees * PI / 180.0;
}

#include "interval.h"
#include "ray.h"
#include "vec3.h"

#endif //RAYTRACER_RTMATH_H
