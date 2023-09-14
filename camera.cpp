#include "camera.h"
#include <thread>
#include <iostream>
#include <fstream>
#include <cassert>

using namespace std;

void camera::render(hittable &world) {
    std::clog << "\rStarting.                 \n";
    init();

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    for (int r = 0; r < image_height; ++r) {
        for (int c = 0; c < image_width; ++c) {
            if(antialiasing) {
                write_color_antialias(std::cout, render_color_antialias(r, c, world), samples_per_pixel);
            } else {
                write_color(std::cout, render_color(r, c, world));
            }
        }
    }
    std::clog << "\rDone.                 \n";
}

void camera::render_multithread(hittable &world) {
    std::clog << "\rStarting.                 \n";
    init();

    int batch_size = image_height / num_threads;
    int batches = (image_height + batch_size - 1) / batch_size;

    vector<vector<color>> pixels(image_height, vector<color>(image_width));
    vector<thread> threads;

    for (int i = 0; i < batches; ++i) {
        thread t([this, &world, &pixels](int batch, int batch_size) {
            int r_start = batch * batch_size;
            int r_end = min(r_start + batch_size, image_height);
            for (int r = r_start; r < r_end; ++r) {
                for(int c = 0; c < image_width; ++c) {
                    pixels[r][c] = render_color_antialias(r, c, world);
                }
            }
        }, i, batch_size);
        threads.push_back(move(t));
    }

    for(auto &t : threads) {
        t.join();
    }

    // Write to file

    ofstream output_file(image_filename);
    output_file << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    assert(output_file.is_open());

    for (int r = 0; r < image_height; ++r) {
        for (int c = 0; c < image_width; ++c) {
            if(antialiasing) {
                write_color_antialias(output_file, pixels[r][c], samples_per_pixel);
            } else {
                write_color(output_file, pixels[r][c]);
            }
        }
    }
    output_file.close();

    std::clog << "\rDone.                 \n";
}

void camera::init() {
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

color camera::ray_color(const ray &r, const hittable &world, uint depth) {
    // if ray bounces for too long, we approximate by returning no light
    if(depth >= max_render_depth) {
        return color(0,0,0);
    }

    // Interval starts at 0.001 to prevent shadow acne.
    // Calculated intersection may be slightly off and bounced ray may be just below the surface,
    // causing the next ray to intersect the surface (and have a very small t value for the intersection)
    hit_record record;
    if(world.hit(r, interval(0.001, INF), record)) {
        color attenuation;
        ray scattered;
        if(record.mat->scatter(r, record, attenuation, scattered)) {
            // Ray bounces until it hits background color
            // More bounces, color value gets smaller, color becomes darker
            return attenuation * ray_color(scattered, world, depth+1);
        }
        // Material omits light directly
        return attenuation;
//            return color(0,0,0);
    }

    // blue sky gradient background
    vec3 unit_dir = unit_vector(r.direction());
    double a = 0.5 * (unit_dir.y() + 1.0); // unit_ray_vec.y transform to [0, 1]
    vec3 white = color(1.0, 1.0, 1.0);
    vec3 blue = color(0.5, 0.7, 1.0);
    return (1.0 - a) * white + a * blue;
}

color camera::render_color(int r, int c, const hittable &world) {
    point3 pixel_center = pixel00_center + c*pixel_delta_u + r*pixel_delta_v;
    vec3 ray_dir = pixel_center - camera_center; // goes from camera to pixel
    ray ry(camera_center, ray_dir);
    return ray_color(ry, world, 0);
}

color camera::render_color_antialias(int r, int c, const hittable &world) {
    point3 pixel_center = pixel00_center + c*pixel_delta_u + r*pixel_delta_v;
    color pixel_color = color(0, 0, 0);
    for(int i = 0; i < samples_per_pixel; ++i) {
        const ray sample = sample_ray(pixel_center);
        color sample_color = ray_color(sample, world, 0);
        pixel_color += sample_color;
    }
    return pixel_color;
}

ray camera::sample_ray(point3 pixel_center) {
    point3 sampled_pixel = sample_pixel_square(pixel_center);

    vec3 dir = sampled_pixel - camera_center;
    return ray(camera_center, dir);
}

point3 camera::sample_pixel_square(point3 pixel_center) {
    double dr = random_double(-0.5, 0.5);
    double dc = random_double(-0.5, 0.5);
    return pixel_center + dr*pixel_delta_v + dc*pixel_delta_u;
}