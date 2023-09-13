#pragma once

#include "hittable.h"
#include "interval.h"

// Keep track of a contact point between a (light) rays 
class sphere: public hittable {
public:
    sphere(point3 center, double radius, std::shared_ptr<material> mat);

    // Detect if a ray will intersect a sphere
    // vector equation of ray: P(t) = O + dt
    // vector equation of sphere: r^2 = (x - C_x)^2 + (y - C_y)^2 + (z - C_z)^2 = (P_s - C) dot (P_s - C)
    // where r is radius, C is center of sphere, P_s is point on sphere
    // We plug in P_s = P(t) solve for t => if there is solution, ray will intersect the sphere at some t
    // Resulting equation is quadratic, we check the discriminant to see if there is a solution
    // negative = no roots => no intersection
    // 0 or positive = at least 1 root => intersection
    // we return t, if t < 0 => no intersection (or intersection is at back of camera)
    bool hit(const ray &r, interval ray_t, hit_record &rec) const override;

private:
    point3 center;
    double radius;
    std::shared_ptr<material> mat;
};
