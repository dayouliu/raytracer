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

// Detect if a ray will intersect a sphere
// vector equation of ray: P(t) = O + dt
// vector equation of sphere: r^2 = (x - C_x)^2 + (y - C_y)^2 + (z - C_z)^2 = (P_s - C) dot (P_s - C)
// where r is radius, C is center of sphere, P_s is point on sphere
// We plug in P_s = P(t) solve for t => if there is solution, ray will intersect the sphere at some t
// Resulting equation is quadratic, we check the discriminant to see if there is a solution
// negative = no roots => no intersection
// 0 or positive = at least 1 root => intersection
// we return t, if t < 0 => no intersection (or intersection is at back of camera)
double ray_sphere_intersect(const ray &r, const point3 &center, double radius) {
    point3 O = r.origin();
    vec3 d = r.direction();
    vec3 oc = O - center;
    // equation for a, b,c in quadratic formula
    double a = dot(d, d);
    double b = dot(2*d, oc);
    double c = dot(oc, oc) - radius*radius;
    double discriminant = b*b - 4*a*c;

    // Does not intersect sphere
    if(discriminant < 0) {
        return -1;
    }
    return (-b - std::sqrt(discriminant)) / (2.0 * a);
}

color ray_color(const ray &r) {
    const vec3 sphere_center = vec3(0, 0, -1);
    const double sphere_radius = 0.5;
    const double t_intersect = ray_sphere_intersect(r, sphere_center, sphere_radius);
    if(t_intersect > 0.0) {
        const vec3 n = unit_vector(r.at(t_intersect) - sphere_center); // normal
        // Transform normal values from domain [-1, 1] => [0, 1] for color
        const color n_color = color(n.x()+1, n.y()+1, n.z()+1) * 0.5;
        return n_color;
    }

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
