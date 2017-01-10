#pragma once
#ifndef SPHEREH
#define SPHEREH

#include "hitable.h" 

class sphere :public hitable {
public:
	sphere() {}
	sphere(vec3 cen, float r, Material *mat) :center(cen), radius(r) , mat(mat){}
	virtual bool hit(const ray& r, float tmin,float tmax, hit_record& rec)const;
private:
	vec3 center;
	float radius;
	Material *mat;

};
bool sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec)const {
	vec3 oc = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = dot(oc, r.direction());
	float c = dot(oc, oc) - radius*radius;
	float discriminant = b*b -  a*c;

	if (discriminant > 0.0f) {
		float temp =( -b - sqrt(discriminant)) / a;
		if (temp < t_max && temp> t_min) {
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			rec.mat_ptr = mat;
			return true;
		}
		temp = (-b + sqrt(discriminant)) / a;
		if (temp < t_max && temp > t_min) {
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			rec.mat_ptr = mat;
			return true;
		}
	}
	return false;
}
class moving_sphere :public hitable {
public:
	moving_sphere() {}
	moving_sphere(vec3 cen0,vec3 cen1,float t0,float t1, float r, Material *mat) :centerstart(cen0),centerend(cen1),timestart(t0),timeend(t1), radius(r), mat(mat) {}
	virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec)const;
	vec3 center(float time)const;
private:
	vec3 centerstart,centerend;
	float timestart, timeend;
	float radius;
	Material *mat;

};
vec3 moving_sphere::center(float time) const{
	return centerstart + ((time - timestart) / (timeend - timestart))*(centerend - centerstart);
}
bool moving_sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec)const {
	vec3 oc = r.origin() - center(r.timer());
	float a = dot(r.direction(), r.direction());
	float b = dot(oc, r.direction());
	float c = dot(oc, oc) - radius*radius;
	float discriminant = b*b - a*c;

	if (discriminant > 0.0f) {
		float temp = (-b - sqrt(discriminant)) / a;
		if (temp < t_max && temp> t_min) {
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center(r.timer())) / radius;
			rec.mat_ptr = mat;
			return true;
		}
		temp = (-b + sqrt(discriminant)) / a;
		if (temp < t_max && temp > t_min) {
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center(r.timer())) / radius;
			rec.mat_ptr = mat;
			return true;
		}
	}
	return false;
}
#endif 