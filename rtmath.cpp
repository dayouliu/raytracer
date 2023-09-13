#include "rtmath.h"

using std::shared_ptr;
using std::make_shared;
using std::sqrt;


const double INF = std::numeric_limits<double>::infinity();
const double PI = M_PI;

// Basic pseudorandom double in range [0, 1), not uniformly distributed
double random_double() {
    return rand() / (RAND_MAX + 1.0);
}

// Basic pseudorandom double in range [min, max), not uniformly distributed
double random_double(double min, double max) {
    return min + (max - min) * random_double();
}

inline double deg_to_rad(double degrees) {
    return degrees * PI / 180.0;
}