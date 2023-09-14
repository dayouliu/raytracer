#include "rtmath.h"

using std::shared_ptr;
using std::make_shared;
using std::sqrt;


const double INF = std::numeric_limits<double>::infinity();
const double PI = M_PI;

double random_double() {
    return rand() / (RAND_MAX + 1.0);
}

double random_double(double min, double max) {
    return min + (max - min) * random_double();
}