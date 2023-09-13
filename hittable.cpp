#include "hittable.h"

void hit_record::set_face_normal(const ray &r, const vec3 &outward_normal) {
    // dot_product = r.direction() dot outward_normal ~ cos(angle between vectors)
    // cos is positive if two vectors face in same direction
    // r.direction() dot outward_normal > 0 => ray is inside the sphere
    // r.direction() dot outward_normal < 0 => ray is outside the sphere
    // ex: if ray is outside, the contact normal should point away from incoming ray (reflect)

    front_face = dot(r.direction(), outward_normal) < 0;
    normal = front_face ? outward_normal : -outward_normal;
}

hittable_list::hittable_list(std::shared_ptr<hittable> obj) {
    hittable_list::add(obj);
}

void hittable_list::clear() {
    objs.clear();
}

void hittable_list::add(std::shared_ptr<hittable> obj) {
    objs.push_back(obj);
}

bool hittable_list::hit(const ray& r, interval ray_t, hit_record& rec) const override {
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