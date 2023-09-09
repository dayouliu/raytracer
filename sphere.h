//
// Created by david on 9/3/23.
//

#ifndef RAYTRACER_SPHERE_H
#define RAYTRACER_SPHERE_H

#include "hittable.h"

// Keep track of a contact point between a (light) rays 
class sphere: public hittable {
public:
    sphere(point3 center, double radius, shared_ptr<material> mat)
        : center(center), radius(radius), mat(mat) {}

    // Detect if a ray will intersect a sphere
    // vector equation of ray: P(t) = O + dt
    // vector equation of sphere: r^2 = (x - C_x)^2 + (y - C_y)^2 + (z - C_z)^2 = (P_s - C) dot (P_s - C)
    // where r is radius, C is center of sphere, P_s is point on sphere
    // We plug in P_s = P(t) solve for t => if there is solution, ray will intersect the sphere at some t
    // Resulting equation is quadratic, we check the discriminant to see if there is a solution
    // negative = no roots => no intersection
    // 0 or positive = at least 1 root => intersection
    // we return t, if t < 0 => no intersection (or intersection is at back of camera)
    bool hit(const ray &r, interval ray_t, hit_record &rec) const override {
        point3 O = r.origin();
        vec3 d = r.direction();
        vec3 oc = O - center;
        // equation for a, b,c in quadratic formula
        double a = dot(d, d);
        double b = dot(2*d, oc);
        double c = dot(oc, oc) - radius*radius;
        double discriminant = b*b - 4*a*c;

        // Does not intersect sphere
        if(discriminant < 0) return false;

        // Find a in range root
        double sqrt_discrim = std::sqrt(discriminant);
        double root = (-b - sqrt_discrim) / (2.0 * a);
        if (!ray_t.surrounds(root)) {
            root = (-b + sqrt_discrim) / (2.0 * a);
            // No roots in range
            if(!ray_t.surrounds(root)) return false;
        }

        rec.t = root;
        rec.point = r.at(root);
        vec3 outward_normal = (rec.point - center) / radius; // as a unit vector
        rec.set_face_normal(r, outward_normal);
        rec.mat = mat;
        return true;
    }

private:
    point3 center;
    double radius;
    shared_ptr<material> mat;
};

#endif //RAYTRACER_SPHERE_H
