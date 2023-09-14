#pragma once

#include "rtmath.h"
#include "hittable.h"
#include "color.h"

class hit_record;

class material {
public:
    // Allows for child destructor to be called for polymorphism
    virtual ~material() = default;

    virtual bool scatter(const ray& r_in, const hit_record &rec, color &attenuation, ray &scattered) const = 0;
};

class normal_gradient : public material {
    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override;
};

class random_diffuse : public material {
public:
    random_diffuse(const color &alb);

    bool scatter(const ray& r_in, const hit_record &rec, color &attenuation, ray &scattered) const override;

private:
    color albedo;
};

class lambertian : public material {
public:
    lambertian(const color &alb);

    bool scatter(const ray& r_in, const hit_record &rec, color &attenuation, ray &scattered) const override;

private:
    color albedo;
};

class metal : public material {
public:
    metal(const color& a);

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override;

private:
    color albedo;
};
