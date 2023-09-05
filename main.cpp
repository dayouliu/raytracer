#include "rtmath.h"
#include "color.h"
#include "hittable.h"
#include "sphere.h"
#include "camera.h"

#include <iostream>

int main() {
    // World objects
    hittable_list world;
    world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
    world.add(make_shared<sphere>(point3(0,-100.5,-1), 100));

    camera cam;
    cam.render(world);
}
