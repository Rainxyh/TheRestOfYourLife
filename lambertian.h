#pragma once
#include "material.h"
#include "texture.h"
#include "onb.h"

class lambertian : public material
{
public:
	lambertian(){};
	lambertian(texture *albedo) : albedo(albedo){};

	virtual float scattering_pdf(const ray &ray_in, const hit_record &rec, ray &scattered) const override
	{
		float cosine = dot(rec.normal, vec::unit_vector(scattered.direction()));
		if (cosine < 0)
			cosine = 0;
		return cosine / M_PI;
	}

	// virtual bool scatter(const ray &ray_in, const hit_record &rec, vec::vec3 &alb, ray &scattered, float &pdf) const override
	virtual bool scatter(const ray &ray_in, const hit_record &hrec, scatter_record& srec) const override
	{
		srec.is_specular=false;
		srec.attenuation=albedo->value(hrec.u,hrec.v,hrec.point);
		srec.pdf_ptr=new cosine_pdf(hrec.normal);
		return true;
	}

	virtual bool scatter(const ray &ray_in, const hit_record &rec, vec::vec3 &alb, ray &scattered, float &pdf) const override
	{
		vec::vec3 direction(0);
		// do{
		// 	direction = random_in_unit_sphere();
		// }while(vec::dot(direction,rec.normal)<0);

		onb uvw;
		uvw.build_from_w(rec.normal);
		direction = uvw.local(random_cosine_direction());

		scattered = ray(rec.point, vec::unit_vector(direction), ray_in.get_time());
		alb = albedo->value(rec.u, rec.v, rec.point);
		pdf = dot(uvw.w(), scattered.direction()) / M_PI;
		return true;
	}

	virtual const material *get_class_type(void) const override
	{
		return this;
	}

	texture *albedo;
};