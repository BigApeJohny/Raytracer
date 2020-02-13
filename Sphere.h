#pragma once

#include <math.h>
#include "Vec3.h"

class Sphere
{
	typedef Vec3<float> Vec3f;

public:
	Vec3f _center;
	float _radius, _radiusPower;
	Vec3f _color, _emColor;
	float _transparency, _reflection;

	Sphere(
		const Vec3f &center,
		const float &radius,
		const Vec3f &color,
		const float &reflection = 0.0f,
		const float &transparency = 0.0f,
		const Vec3f &emColor = 0
	) :
		_center(center),
		_radius(radius),
		_radiusPower(radius * radius),
		_color(color),
		_emColor(emColor),
		_transparency(transparency),
		_reflection(reflection) {}
public:
	bool intersect(const Vec3f &origin, const Vec3f &direction, float &t0, float &t1) const {
		Vec3f l = _center - origin;
		float tca = l.dot(direction);
		if (tca < 0) {
			return false;
		}

		float d2 = l.dot(l) - tca * tca;

		if (d2 > _radiusPower) {
			return false;
		}

		float thc = sqrt(_radiusPower - d2);
		t0 = tca - thc;
		t1 = tca + thc;

		return true;
	}
};

