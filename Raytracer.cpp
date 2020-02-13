#include <iostream>
#include <fstream> 

#include "Vec3.h"
#include "Ray.h"
#include "InputReader.h"

#define PI 3.141592653589793 

typedef Vec3<float> Vec3f;

const int width = 512;
const int height = 512;
int maxDepth = 5;
Vec3f camera = Vec3f(0);

void render(int width, int height, const std::vector<Sphere>& spheres) {
    Vec3f* image = new Vec3f[width * height];
    Vec3f* pixel = image;
    float invWidth = 1 / float(width);
    float invHeight = 1 / float(height);
    float fov = 30;
    float aspectRatio = width / float(height);
    float angle = tan(PI * 0.5 * fov / 180.0f);

    Ray* ray = new Ray(maxDepth);

    for (unsigned i = 0; i < height; i++) {
        for (unsigned j = 0; j < width; j++, pixel++) {
            float xx = (2 * ((j + 0.5) * invWidth) - 1) * angle * aspectRatio;
            float yy = (1 - 2 * ((i + 0.5) * invHeight)) * angle;
            Vec3f direction(xx, yy, -1);
            direction.normalize();
            *pixel = ray->trace(camera, direction, spheres, 5);
        }
    }

    std::ofstream ofs("./untitled.ppm", std::ios::out | std::ios::binary);
    ofs << "P6\n" << width << " " << height << "\n255\n";
    for (unsigned i = 0; i < width * height; ++i) {
        ofs << (unsigned char)(std::min(float(1), image[i].x) * 255) <<
            (unsigned char)(std::min(float(1), image[i].y) * 255) <<
            (unsigned char)(std::min(float(1), image[i].z) * 255);
    }
    ofs.close();
    delete[] image;
}

int main()
{

    InputReader ir = InputReader("test.txt");
    ir.read();

    
    std::vector<Sphere> spheres;
    // position, radius, surface color, reflectivity, transparency, emission color
    spheres.push_back(Sphere(Vec3f(0.0, -10004, -20), 10000, Vec3f(0.20, 0.20, 0.20), 0, 0.0));
    spheres.push_back(Sphere(Vec3f(0.0, 0, -20), 4, Vec3f(1.00, 0.32, 0.36), 0, 0.5));
    spheres.push_back(Sphere(Vec3f(5.0, -1, -15), 2, Vec3f(0.90, 0.76, 0.46), 0, 0.0));
    spheres.push_back(Sphere(Vec3f(5.0, 0, -25), 3, Vec3f(0.65, 0.77, 0.97), 0, 0.0));
    spheres.push_back(Sphere(Vec3f(-5.5, 0, -15), 3, Vec3f(0.90, 0.90, 0.90), 0, 0.0));
    // light
    spheres.push_back(Sphere(Vec3f(0.0, 20, -30), 3, Vec3f(0.00, 0.00, 0.00), 0, 0.0, Vec3f(3)));
    render(width, height, spheres);

    return 0;
}
