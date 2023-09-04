cmake_minimum_required(VERSION 3.26)
project(raytracer)

set(CMAKE_CXX_STANDARD 17)

add_executable(raytracer main.cpp
        vec3.h
        color.h
        ray.h
        hittable.h
        sphere.h
        rtmath.h
        interval.h
        camera.h
        test_ppm.cpp
)
