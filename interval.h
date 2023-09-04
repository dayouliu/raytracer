//
// Created by david on 9/3/23.
//

#ifndef RAYTRACER_INTERVAL_H
#define RAYTRACER_INTERVAL_H

#include "rtmath.h"

class interval {
public:
    double min, max;
    interval() : min(+INF), max(-INF) {} // Default interval is empty

    interval(double _min, double _max) : min(_min), max(_max) {}

    bool contains(double x) const {
        return min <= x && x <= max;
    }

    bool surrounds(double x) const {
        return min < x && x < max;
    }

    static const interval empty, universe;
};

const interval interval::empty = interval(+INF, -INF);
const interval interval::universe = interval(-INF, +INF);

#endif //RAYTRACER_INTERVAL_H
