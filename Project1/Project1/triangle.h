#pragma once
#ifndef TRIANGLEH
#define TRIANGLEH

#include "hitable.h"

class triangle :public hitable {
public:
	triangle() {}
	triangle(vec3 p0,vec3 p1, vec3 p2, Material *mat):point0(p0),point1(p1),point2(p2){}
	virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec)const;
private:
	vec3 point0, point1, point2;
};
bool triangle::hit(const ray& r, float t_min, float t_max, hit_record& rec)const {
	vec3 t0 = point1 - point0;
	vec3 t1 = point2 - point0;
	vec3 normal = unit_vector(cross(t0, t1));
}
#endif // !TRIANGLEH
