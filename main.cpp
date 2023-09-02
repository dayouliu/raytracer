#include "color.h"
#include "vec3.h"
#include "ray.h"

#include <iostream>

void testPPM() {
    // Image dim
    int image_width = 256;
    int image_height = 256;

    // Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    for (int j = 0; j < image_height; ++j) {
        for (int i = 0; i < image_width; ++i) {
            std::clog << "\rscanlines remaining: " << (image_height-j) << ' ' << std::flush;

            auto r = double(i) / (image_width-1);
            auto g = double(j) / (image_height-1);
            auto b = 0;

            // Scale up our fractions to 0-255
            int r_int = static_cast<int>(r * 255.999);
            int g_int = static_cast<int>(g * 255.999);
            int b_int = b;

            std::cout << r_int << ' ' << g_int << ' ' << b_int << '\n';
        }
    }
    std::clog << "\rDone.                 \n";
}

color ray_color(const ray &r) {
    vec3 unit_dir = unit_vector(r.direction());
    // unit_ray_vec.y => 0.5
    double a = 0.5 * (unit_dir.y() + 1.0);
    vec3 white = color(1.0, 1.0, 1.0);
    vec3 blue = color(0.5, 0.7, 1.0);
    return (1.0 - a) * white + a * blue;
}

int main() {
    // Aspect ratio
    double aspect_ratio_ideal = 16.0 / 9.0;

    // Image dimensions in pixels (pixel dimensions)
    int image_width = 400;
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

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    for (int r = 0; r < image_height; ++r) {
        for (int c = 0; c < image_width; ++c) {
            point3 pixel_center = pixel00_center + c*pixel_delta_u + r*pixel_delta_v;
            vec3 ray_dir = pixel_center - camera_center; // goes from camera to pixel
            ray ry(camera_center, ray_dir);
            color pixel_color = ray_color(ry);
            write_color(std::cout, pixel_color);
        }
    }
    std::clog << "\rDone.                 \n";
}
