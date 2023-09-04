#include "rtmath.h"
#include "color.h"
#include "hittable.h"
#include "sphere.h"

#include <iostream>



int main() {


    // World objects
    hittable_list world;
    world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
    world.add(make_shared<sphere>(point3(0,-100.5,-1), 100));


    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    for (int r = 0; r < image_height; ++r) {
        for (int c = 0; c < image_width; ++c) {
            point3 pixel_center = pixel00_center + c*pixel_delta_u + r*pixel_delta_v;
            vec3 ray_dir = pixel_center - camera_center; // goes from camera to pixel
            ray ry(camera_center, ray_dir);
            color pixel_color = ray_color(ry, world);
            write_color(std::cout, pixel_color);
        }
    }

    std::clog << "\rDone.                 \n";
}
