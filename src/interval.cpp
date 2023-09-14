#include "interval.h"

interval::interval() : min(+INF), max(-INF) {}

interval::interval(double _min, double _max) : min(_min), max(_max) {}

bool interval::contains(double x) const {
    return min <= x && x <= max;
}

bool interval::surrounds(double x) const {
    return min < x && x < max;
}

double interval::clamp(double x) const {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

const interval interval::empty = interval(+INF, -INF);
const interval interval::universe = interval(-INF, +INF);
