//
// Created by david on 8/26/23.
//

#ifndef RAYTRACER_VEC3_H
#define RAYTRACER_VEC3_H

#include <cmath>
#include <iostream>
#include "rtmath.h"

using std::sqrt;

class vec3 {
public:
    double e[3];

    vec3() : e{0,0,0} {}
    vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

    double x() const { return e[0]; }
    double y() const { return e[1]; }
    double z() const { return e[2]; }

    vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
    double operator[](int i) const { return e[i]; }
    double& operator[](int i) { return e[i]; }

    vec3& operator+=(const vec3 &v) {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    vec3& operator*=(double t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    vec3& operator/=(double t) {
        return *this *= 1/t;
    }

    double length() const {
        return sqrt(length_squared());
    }

    double length_squared() const {
        return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
    }

    static vec3 random() {
        return vec3(random_double(), random_double(), random_double());
    }

    static vec3 random(double min, double max) {
        return vec3(random_double(min,max), random_double(min,max), random_double(min,max));
    }

    bool near_zero() const {
        double s = 1e-8;
        return (abs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
    }
};

// point3 is just an alias for vec3, but useful for geometric clarity in the code.
using point3 = vec3;


// Vector Utility Functions

inline std::ostream& operator<<(std::ostream &out, const vec3 &v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline vec3 operator+(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline vec3 operator-(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline vec3 operator*(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator*(double t, const vec3 &v) {
    return vec3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

inline vec3 operator*(const vec3 &v, double t) {
    return t * v;
}

inline vec3 operator/(vec3 v, double t) {
    return (1/t) * v;
}

inline double dot(const vec3 &u, const vec3 &v) {
    return u.e[0] * v.e[0]
           + u.e[1] * v.e[1]
           + u.e[2] * v.e[2];
}

inline vec3 cross(const vec3 &u, const vec3 &v) {
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline vec3 unit_vector(vec3 v) {
    return v / v.length();
}

void test(int p) {

}

// Uses the rejection method
// statistically likely to pick a valid vector (unit sphere / unit cube)
inline vec3 random_vec_in_unit_sphere() {
    int b = 1;
    const int *a = &b;
    test(b);

    while(true) {
        vec3 v = vec3::random(-1, 1);
        if(v.length_squared() < 1) return v;
    }
}

// Picking a unit vector this way will lead to bias because the volume space is a cube,
// there will be more bias towards the diagonals
// https://towardsdatascience.com/the-best-way-to-pick-a-unit-vector-7bd0cc54f9b
//inline vec3 random_unit_vec() {
//    return unit_vector(vec3::random(-1, 1));
//}

inline vec3 random_unit_vec() {
    return unit_vector(random_vec_in_unit_sphere());
}

inline vec3 random_unit_vec_on_hemisphere(const vec3 &normal) {
    vec3 v = random_unit_vec();
    if(dot(normal, v) < 0) v *= -1; // not in same dir
    return v;
}

// Recall dot product is magnitude of the projection of v on normal scaled by |n| (v dot n = |n| * |v| cos theta
// where theta is the angle between n and v.
// Noted that n is normalized and a unit vector, so v dot n = |v| cos theta = b.
// Geometrically, v - 2b will give us the reflected vector.
inline vec3 reflect(const vec3 &v, const vec3 &normal) {
    return v - 2 * dot(v, normal) * normal; // normal must be a unit vector
}


#endif //RAYTRACER_VEC3_H

