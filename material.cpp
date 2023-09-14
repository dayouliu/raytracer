#include "material.h"

bool normal_gradient::scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const {
    // Convert normal into a color [-1, 1] => [0, 1]
    color normal_gradient = 0.5 * (rec.normal + color(1, 1, 1));
    attenuation = normal_gradient;
    return false;
}

random_diffuse::random_diffuse(const color &alb) : albedo(alb) {}

bool random_diffuse::scatter(const ray& r_in, const hit_record &rec,
             color &attenuation, ray &scattered) const {
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

lambertian::lambertian(const color &alb) : albedo(alb) {}

bool lambertian::scatter(const ray& r_in, const hit_record &rec,
             color &attenuation, ray &scattered) const {
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

metal::metal(const color& a) : albedo(a) {}

bool metal::scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const {
    vec3 reflected = reflect(r_in.direction(), rec.normal);
    scattered = ray(rec.point, unit_vector(reflected));
    attenuation = albedo;
    return true;
}