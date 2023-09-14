cmake_minimum_required(VERSION 3.26)
project(raytracer)

set(CMAKE_CXX_STANDARD 17)

add_executable(raytracer main.cpp
        src/vec3.h
        src/color.h
        src/ray.h
        src/hittable.h
        src/sphere.h
        src/rtmath.h
        src/interval.h
        src/camera.h
        test/test_ppm.cpp
        src/material.h
        src/color.cpp
        src/hittable.cpp
        src/interval.cpp
        src/material.cpp
        src/ray.cpp
        src/rtmath.cpp
        src/sphere.cpp
        src/vec3.cpp
        src/camera.cpp
)
