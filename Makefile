.PHONY: mytarget

ppm:
	/snap/clion/249/bin/cmake/linux/x64/bin/cmake --build /home/david/CLionProjects/raytracer/cmake-build-debug --target raytracer -j 14
	cmake-build-debug/raytracer > cmake-build-debug/image.ppm
