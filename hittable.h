//
// Created by david on 9/3/23.
//

#ifndef RAYTRACER_HITTABLE_H
#define RAYTRACER_HITTABLE_H

#include "ray.h"
#include <memory>
#include <vector>

class material;

class hit_record {
public:
    point3 point; // hit contact point
    vec3 normal; // as a unit vector
    shared_ptr<material> mat;
    double t; // ray(t) = A + bt
    bool front_face; // if ray is hitting the outward face of object

    // Sets all contact point normal vector to point outwards from shape
    // as normals can have two directions.
    // outward_normal assumed to have unit length.
    void set_face_normal(const ray &r, const vec3 &outward_normal) {
        // dot_product = r.direction() dot outward_normal ~ cos(angle between vectors)
        // cos is positive if two vectors face in same direction
        // r.direction() dot outward_normal > 0 => ray is inside the sphere
        // r.direction() dot outward_normal < 0 => ray is outside the sphere
        // ex: if ray is outside, the contact normal should point away from incoming ray (reflect)

        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class hittable {
public:
    virtual ~hittable() = default;
    virtual bool hit(const ray &r, interval ray_t, hit_record &record) const = 0;
};

class hittable_list : public hittable {
public:
    std::vector<std::shared_ptr<hittable>> objs;

    hittable_list() {}

    hittable_list(std::shared_ptr<hittable> obj) {
        add(obj);
    }

    void clear() {
        objs.clear();
    }

    void add(std::shared_ptr<hittable> obj) {
        objs.push_back(obj);
    }

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        hit_record temp_record;
        bool hit_anything = false;
        double closest_so_far = ray_t.max;

        for(const auto &obj: objs) {
            bool hit = obj->hit(r, ray_t, temp_record);
            if(hit && temp_record.t < closest_so_far) {
                hit_anything = true;
                closest_so_far = temp_record.t;
                rec = temp_record;
            }
        }

        return hit_anything;
    }
};

#endif //RAYTRACER_HITTABLE_H
