//
// Created by david on 9/4/23.
//

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