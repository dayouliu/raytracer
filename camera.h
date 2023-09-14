//
// Created by david on 9/4/23.
//

#pragma once

#include "rtmath.h"
#include "color.h"
#include "hittable.h"
#include "material.h"

// In the real world, light comes to the camera.
// In our simulation, our rays trace backwards from camera to world.
// Advantage is we only trace the light rays we need to render the image in our camera.
class camera {
public:
    double aspect_ratio_ideal = 16.0 / 9.0;
    int image_width = 400;
    int samples_per_pixel = 100;
    bool antialiasing = true; // considers colors of multiple rays per pixel
    int max_render_depth = 50; // limits number of light ray bounces

    int num_threads = 8; // Rendering threads
    std::string image_filename = "image.ppm";

    void render(hittable &world);
    void render_multithread(hittable &world);

private:
    int image_height;
    point3 pixel00_center;
    vec3 pixel_delta_u, pixel_delta_v; // world unit pixel dimensions
    point3 camera_center;

    void init();

    color ray_color(const ray &r, const hittable &world, uint depth);

    color render_color(int r, int c, const hittable &world);

    // Antiliasing samples colors from surrounding pixels as well.
    // Motivation: pixels for faraway checkerboard will appear grey instead of
    // black and/or white, mimicking our eyes
    color render_color_antialias(int r, int c, const hittable &world);

    ray sample_ray(point3 pixel_center);

    // randomly samples closest nearby pixels
    point3 sample_pixel_square(point3 pixel_center);
};
