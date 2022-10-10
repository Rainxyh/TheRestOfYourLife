#pragma once
#include "ray.h"
#include "aabb.h"

class material;

struct hit_record
{
	float t;
	float u;
	float v;
	vec::vec3 point;
	vec::vec3 normal;
	material *mat_ptr;
};

class hitable
{
public:
	virtual bool hit(const ray &ray, float t_min, float t_max, hit_record &rec) const = 0;
	virtual bool bounding_box(float t0, float t1, aabb &bbox) const = 0;
    virtual float pdf_value(const vec::vec3& o,const vec::vec3& v)const{return 0.0;}
    virtual vec::vec3 random(const vec::vec3& o)const {return vec::vec3(1,0, 0);}
};

void get_sphere_uv(float &u, float &v, const vec::vec3 &point)
{
    float phi = atan2(point.z(), point.x());
    float theta = asin(point.y());
    u = 1 - (phi + M_PI) / (2 * M_PI);
    v = (theta + M_PI / 2) / M_PI;
    // float phi = atan2(point.y(), point.x());
    // float theta = acos(point.z());
    // u = phi / (2 * M_PI);
    // v = theta / M_PI;
}

// class hitable_pdf:public pdf{

// }