#pragma once
#include "material.h"

class metal : public material
{
public:
	metal(){};
	metal(const vec::vec3 &albedo) : albedo(albedo) { fuzz = 0.1; };
	metal(const vec::vec3 &albedo, float f) : albedo(albedo)
	{
		if (f < 1)
			fuzz = f;
		else
			fuzz = 1;
	};

	virtual bool scatter(const ray &ray_in, const hit_record &hrec, scatter_record &srec) const override
	{
		vec::vec3 reflected = reflect(vec::unit_vector(ray_in.direction()), hrec.normal);
		srec.specular_ray = ray(hrec.point, reflected + fuzz * random_in_unit_sphere());
		srec.attenuation = albedo;
		srec.is_specular = true;
		srec.pdf_ptr = 0;
		return true;
	}

	virtual bool scatter(const ray &ray_in, const hit_record &rec, vec::vec3 &alb, ray &scattered, float &pdf) const override
	{
		vec::vec3 reflected = reflect(vec::unit_vector(ray_in.dir), rec.normal);
		scattered = ray(rec.point, reflected + fuzz * random_in_unit_sphere(), ray_in.get_time());
		alb = albedo;
		return (dot(scattered.direction(), rec.normal) > 0);
	}

	virtual const material *get_class_type(void) const override
	{
		return this;
	}

	vec::vec3 albedo;
	float fuzz;
};