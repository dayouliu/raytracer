#pragma once

#include <cmath>
#include <limits>
#include <memory>
#include "vec3.h"

extern const double INF;
extern const double PI;

// Basic pseudorandom double in range [0, 1), not uniformly distributed
double random_double();

// Basic pseudorandom double in range [min, max), not uniformly distributed
double random_double(double min, double max);

inline double deg_to_rad(double degrees) {
    return degrees * PI / 180.0;
}
