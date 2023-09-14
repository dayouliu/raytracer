#include "color.h"
#include "interval.h"

void write_color(std::ostream &out, color pixel_color) {
    // Write the translated [0,255] value of each color component [0, 1].
    out << static_cast<int>(255.999 * pixel_color.x()) << ' '
        << static_cast<int>(255.999 * pixel_color.y()) << ' '
        << static_cast<int>(255.999 * pixel_color.z()) << '\n';
}

void write_color_antialias(std::ostream &out, color pixel_color, int samples_per_pixel) {
    double r = pixel_color.x();
    double g = pixel_color.y();
    double b = pixel_color.z();

    // Scale images due to sampling multiple pixels for anti-aliasing
    double scale = 1.0 / samples_per_pixel;
    r *= scale;
    g *= scale;
    b *= scale;

    // Convert linear to gamma as most image viewers expect images to be in gamma space
    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);

    // Write the translated [0,255] value of each color component [0, 1].
    static const interval intensity(0.000, 0.999);
    r = static_cast<int>(255.999 * intensity.clamp(r));
    g = static_cast<int>(255.999 * intensity.clamp(g));
    b = static_cast<int>(255.999 * intensity.clamp(b));

    out << r << ' ' << g << ' ' << b << '\n';
}