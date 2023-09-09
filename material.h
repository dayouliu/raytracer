//
// Created by david on 9/8/23.
//

#ifndef RAYTRACER_MATERIAL_H
#define RAYTRACER_MATERIAL_H

#include "rtmath.h"
#include "hittable.h"
#include "color.h"

class hit_record;

class material {
public:
    // Allows for child destructor to be called for
    virtual ~material() = default;

    virtual bool scatter(const ray& r_in, const hit_record &rec,
                         color &attenuation, ray &scattered)
                         const = 0;
};

//class normal_gradient : public material {
//    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
//    const override {
//        // Convert normal into a color
//        color normal_gradient = 0.5 * (rec.normal + color(1, 1, 1));
//        attenuation = normal_gradient;
//        return true;
//    }
//};

class random_diffuse : public material {
public:
    random_diffuse(const color &alb) : albedo(alb) {}

    bool scatter(const ray& r_in, const hit_record &rec,
                 color &attenuation, ray &scattered)
    const override {
        // Simple diffuse material
        // Light ray is scattered randomly across surface
        vec3 scatter_dir = rec.normal;

        // Catch degenerate scatter direction
        // Zero vector direction can result in NaN calculations later on
        if (scatter_dir.near_zero())
            scatter_dir = rec.normal;

        scattered = ray(rec.point, scatter_dir);
        attenuation = albedo;
        return true; // Always scatter and attenuate instead of probability of being absorbed
    }

private:
    color albedo;
};

class lambertian : public material {
public:
    lambertian(const color &alb) : albedo(alb) {}

    bool scatter(const ray& r_in, const hit_record &rec,
                 color &attenuation, ray &scattered)
                 const override {
        // Lambertian Reflection
        // More realistically models the real world. Reflected rays biased towards normal.
        vec3 scatter_dir = rec.normal + random_unit_vec();

        // Catch degenerate scatter direction
        // Zero vector direction can result in NaN calculations later on
        if (scatter_dir.near_zero())
            scatter_dir = rec.normal;

        scattered = ray(rec.point, scatter_dir);
        attenuation = albedo;
        return true; // Always scatter and attenuate instead of probability of being absorbed
    }

private:
    color albedo;
};

class metal : public material {
public:
    metal(const color& a) : albedo(a) {}

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
    const override {
        vec3 reflected = reflect(r_in.direction(), rec.normal);
        scattered = ray(rec.point, unit_vector(reflected));
        attenuation = albedo;
        return true;
    }

private:
    color albedo;
};

#endif //RAYTRACER_MATERIAL_H
