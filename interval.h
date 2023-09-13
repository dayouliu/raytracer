#pragma once

#include "rtmath.h"

class interval {
public:
    double min, max;
    interval() : min(+INF), max(-INF) {} // Default interval is empty

    interval(double _min, double _max) : min(_min), max(_max) {}

    bool contains(double x) const;

    bool surrounds(double x) const;

    double clamp(double x) const;

    static const interval empty, universe;
};
