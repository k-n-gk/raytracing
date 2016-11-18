#pragma once
#ifndef CAMERAH
#define CAMERAH
#define _USE_MATH_DEFINES
#include "ray.h"

class camera {
public:
	camera(vec3 lookfrom,vec3 lookat,vec3 vup, float vfov ,float aspect) {
		vec3 u, v, w;
		float theta = vfov*(float)M_PI / 180;
		float half_height = tan(theta / 2);
		float half_width = aspect * half_height;
		origin = lookfrom;
		w = unit_vector(lookfrom - lookat);
		u = unit_vector(cross(vup, w));
		v = cross(w, u);
		//lower_left_corner = vec3(-half_width, -half_height, -1.0f);
		lower_left_corner = origin - half_width*u - half_height*v - w;
		horizontal = 2.0f * half_width * u;
		vertical = 2.0f * half_height * v;
		
	}

	ray get_ray(float u, float v) { return ray(origin, lower_left_corner + u * horizontal + v * vertical - origin); }
private:
	vec3 lower_left_corner;
	vec3 horizontal;
	vec3 vertical;
	vec3 origin;
};
#endif