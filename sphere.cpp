#include "sphere.h"

sphere::sphere(point3 center, double radius, std::shared_ptr<material> mat)
            : center(center), radius(radius), mat(mat) {}

bool sphere::hit(const ray &r, interval ray_t, hit_record &rec) const {
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
