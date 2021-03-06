#pragma once

#include <vector>
#include <algorithm>
#include "Sphere.h"
#include "Vec3.h"

#define INFINITY 1e8

class Ray
{
	typedef Vec3<float> Vec3f;

public:

	float _maxDepth;

	Ray(const float maxDepth) : _maxDepth(maxDepth) {}

	float mix(const float& a, const float& b, const float& mix)
	{
		return b * mix + a * (1 - mix);
	}

    Vec3f trace(
        const Vec3f& rayorig,
        const Vec3f& raydir,
        const std::vector<Sphere>& spheres,
        const int& depth)
    {
        //if (raydir.length() != 1) std::cerr << "Error " << raydir << std::endl;
        float tnear = INFINITY;
        const Sphere* sphere = NULL;
        // find intersection of this ray with the sphere in the scene
        for (unsigned i = 0; i < spheres.size(); ++i) {
            float t0 = INFINITY, t1 = INFINITY;
            if (spheres[i].intersect(rayorig, raydir, t0, t1)) {
                if (t0 < 0) t0 = t1;
                if (t0 < tnear) {
                    tnear = t0;
                    sphere = &spheres[i];
                }
            }
        }
        // if there's no intersection return black or background color
        if (!sphere) return Vec3f(2);
        Vec3f surfaceColor = 0; // color of the ray/surfaceof the object intersected by the ray 
        Vec3f phit = rayorig + raydir * tnear; // point of intersection 
        Vec3f nhit = phit - sphere->_center; // normal at the intersection point 
        nhit.normalize(); // normalize normal direction 
        // If the normal and the view direction are not opposite to each other
        // reverse the normal direction. That also means we are inside the sphere so set
        // the inside bool to true. Finally reverse the sign of IdotN which we want
        // positive.
        float bias = 1e-4; // add some bias to the point from which we will be tracing 
        bool inside = false;
        if (raydir.dot(nhit) > 0) nhit = -nhit, inside = true;
        if ((sphere->_transparency > 0 || sphere->_reflection > 0) && depth < 5) {
            float facingratio = -raydir.dot(nhit);
            // change the mix value to tweak the effect
            float fresneleffect = mix(pow(1 - facingratio, 3), 1, 0.1);
            // compute reflection direction (not need to normalize because all vectors
            // are already normalized)
            Vec3f refldir = raydir - nhit * 2 * raydir.dot(nhit);
            refldir.normalize();
            Vec3f reflection = trace(phit + nhit * bias, refldir, spheres, depth + 1);
            Vec3f refraction = 0;
            // if the sphere is also transparent compute refraction ray (transmission)
            if (sphere->_transparency) {
                float ior = 1.1, eta = (inside) ? ior : 1 / ior; // are we inside or outside the surface? 
                float cosi = -nhit.dot(raydir);
                float k = 1 - eta * eta * (1 - cosi * cosi);
                Vec3f refrdir = raydir * eta + nhit * (eta * cosi - sqrt(k));
                refrdir.normalize();
                refraction = trace(phit - nhit * bias, refrdir, spheres, depth + 1);
            }
            // the result is a mix of reflection and refraction (if the sphere is transparent)
            surfaceColor = (
                reflection * fresneleffect +
                refraction * (1 - fresneleffect) * sphere->_transparency) * sphere->_color;
        }
        else {
            // it's a diffuse object, no need to raytrace any further
            for (unsigned i = 0; i < spheres.size(); ++i) {
                if (spheres[i]._emColor.x > 0) {
                    // this is a light
                    Vec3f transmission = 1;
                    Vec3f lightDirection = spheres[i]._center - phit;
                    lightDirection.normalize();
                    for (unsigned j = 0; j < spheres.size(); ++j) {
                        if (i != j) {
                            float t0, t1;
                            if (spheres[j].intersect(phit + nhit * bias, lightDirection, t0, t1)) {
                                transmission = 0;
                                break;
                            }
                        }
                    }
                    surfaceColor += sphere->_color * transmission *
                        std::max(float(0), nhit.dot(lightDirection)) * spheres[i]._emColor;
                }
            }
        }

        return surfaceColor + sphere->_emColor;
    }
};

