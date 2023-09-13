#pragma once

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
    void set_face_normal(const ray &r, const vec3 &outward_normal);
};

class hittable {
public:
    virtual ~hittable() = default;
    virtual bool hit(const ray &r, interval ray_t, hit_record &record) const = 0;
};

class hittable_list : public hittable {
public:
    std::vector<std::shared_ptr<hittable>> objs;

    hittable_list(std::shared_ptr<hittable> obj);

    void clear();

    void add(std::shared_ptr<hittable> obj);

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override;
};
