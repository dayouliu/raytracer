//
// Created by david on 9/4/23.
//

#ifndef RAYTRACER_CAMERA_H
#define RAYTRACER_CAMERA_H

#include "rtmath.h"
#include "color.h"
#include "hittable.h"

// In the real world, light comes to the camera.
// In our simulation, our rays trace backwards from camera to world.
// Advantage is we only trace the light rays we need to render the image in our camera.
class camera {
public:
    double aspect_ratio_ideal = 16.0 / 9.0;
    int image_width = 400;
    int samples_per_pixel = 10;
    bool antialiasing = true; // considers colors of multiple rays per pixel

    void render(hittable &world) {
        init();

        std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
        for (int r = 0; r < image_height; ++r) {
            for (int c = 0; c < image_width; ++c) {
                if(antialiasing) {
                    render_color_antialias(r, c, world);
                } else {
                    render_color(r, c, world);
                }
            }
        }
        std::clog << "\rDone.                 \n";
    }

private:
    int image_height;
    point3 pixel00_center;
    vec3 pixel_delta_u, pixel_delta_v; // world unit pixel dimensions
    point3 camera_center;

    void init() {
        // Image dimensions in pixels (pixel dimensions)
        image_height = std::max(1, static_cast<int>(image_width / aspect_ratio_ideal));
        double aspect_ratio_actual = static_cast<double>(image_width) / image_height;

        // Camera/viewport dimensions (world dimensions)
        // Uses actual world coordinate dimensions
        double viewport_height = 2.0;
        double viewport_width = viewport_height * aspect_ratio_actual;
        double focal_length = 1.0;
        vec3 focal_vec = vec3(0, 0, -focal_length);
        camera_center = point3(0, 0, 0);

        // Viewport dimensions (world dimensions)
        // u in x dimension, v in the y dimension
        vec3 viewport_u = vec3(viewport_width, 0, 0);
        vec3 viewport_v = vec3(0, -viewport_height, 0);

        // Pixel delta dimensions (world dimensions)
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // Upper-left coordinates (world dimensions)
        point3 viewport_top_left = camera_center + focal_vec - viewport_u/2 - viewport_v/2;
        pixel00_center = viewport_top_left + 0.5 * pixel_delta_u + 0.5 * pixel_delta_v;
    }

    color ray_color(const ray &r, const hittable &world) {
        hit_record record;

        // sphere normal gradient
//        if(world.hit(r, interval(0, INF), record)) {
//            auto normal_gradient = 0.5 * (record.normal + color(1, 1, 1));
//            return normal_gradient;
//        }

        // diffuse material
        if(world.hit(r, interval(0, INF), record)) {
            vec3 dir = random_unit_vec_on_hemisphere(record.normal);

            // ray bounces until it hits background color
            // more bounces, color value gets smaller, color becomes darker
            return 0.5 * ray_color(ray(record.point, dir), world);
        }

        // blue sky gradient background
        vec3 unit_dir = unit_vector(r.direction());
        double a = 0.5 * (unit_dir.y() + 1.0); // unit_ray_vec.y transform to [0, 1]
        vec3 white = color(1.0, 1.0, 1.0);
        vec3 blue = color(0.5, 0.7, 1.0);
        return (1.0 - a) * white + a * blue;
    }

    void render_color(int r, int c, hittable &world) {
        point3 pixel_center = pixel00_center + c*pixel_delta_u + r*pixel_delta_v;
        vec3 ray_dir = pixel_center - camera_center; // goes from camera to pixel
        ray ry(camera_center, ray_dir);
        color pixel_color = ray_color(ry, world);
        write_color(std::cout, pixel_color);
    }

    void render_color_antialias(int r, int c, hittable &world) {
        point3 pixel_center = pixel00_center + c*pixel_delta_u + r*pixel_delta_v;
        color pixel_color = color(0, 0, 0);
        for(int i = 0; i < samples_per_pixel; ++i) {
            const ray sample = sample_ray(pixel_center);
            color sample_color = ray_color(sample, world);
            pixel_color += sample_color;
        }
        write_color_antialias(std::cout, pixel_color, samples_per_pixel);
    }

    ray sample_ray(point3 pixel_center) {
        point3 sampled_pixel = sample_pixel_square(pixel_center);

        vec3 dir = sampled_pixel - camera_center;
        return ray(camera_center, dir);
    }

    // randomly samples closest nearby pixels
    point3 sample_pixel_square(point3 pixel_center) {
        double dr = random_double(-0.5, 0.5);
        double dc = random_double(-0.5, 0.5);
        return pixel_center + dr*pixel_delta_v + dc*pixel_delta_u;
    }
};

#endif //RAYTRACER_CAMERA_H
