//
// Created by david on 9/4/23.
//

#ifndef RAYTRACER_CAMERA_H
#define RAYTRACER_CAMERA_H

#include "rtmath.h"
#include "color.h"
#include "hittable.h"

class camera {
public:
    double aspect_ratio_ideal = 16.0 / 9.0;
    int image_width = 400;

    void render() {

    }

private:
    void init() {
        // Image dimensions in pixels (pixel dimensions)
        int image_height = std::max(1, static_cast<int>(image_width / aspect_ratio_ideal));
        double aspect_ratio_actual = static_cast<double>(image_width) / image_height;

        // Camera/viewport dimensions (world dimensions)
        // Uses actual world coordinate dimensions
        double viewport_height = 2.0;
        double viewport_width = viewport_height * aspect_ratio_actual;
        double focal_length = 1.0;
        vec3 focal_vec = vec3(0, 0, -focal_length);
        point3 camera_center = point3(0, 0, 0);

        // Viewport dimensions (world dimensions)
        // u in x dimension, v in the y dimension
        vec3 viewport_u = vec3(viewport_width, 0, 0);
        vec3 viewport_v = vec3(0, -viewport_height, 0);

        // Pixel delta dimensions (world dimensions)
        vec3 pixel_delta_u = viewport_u / image_width;
        vec3 pixel_delta_v = viewport_v / image_height;

        // Upper-left coordinates (world dimensions)
        point3 viewport_top_left = camera_center + focal_vec - viewport_u/2 - viewport_v/2;
        point3 pixel00_center = viewport_top_left + 0.5 * pixel_delta_u + 0.5 * pixel_delta_v;
    }

    color ray_color(const ray &r, const hittable &world) {
        hit_record rec;
        if(world.hit(r, interval(0, INF), rec)) {
            auto normal_gradient = 0.5 * (rec.normal + color(1, 1, 1));
            return normal_gradient;
        }

        vec3 unit_dir = unit_vector(r.direction());
        double a = 0.5 * (unit_dir.y() + 1.0); // unit_ray_vec.y transform to [0, 1]
        vec3 white = color(1.0, 1.0, 1.0);
        vec3 blue = color(0.5, 0.7, 1.0);
        return (1.0 - a) * white + a * blue;
    }
};

#endif //RAYTRACER_CAMERA_H
