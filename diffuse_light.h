#pragma once
#include "material.h"
#include "texture.h"

class diffuse_light : public material
{
public:
    diffuse_light(){}; 
    diffuse_light(texture *emit) : emit(emit){};
    virtual float scattering_pdf(const ray &ray_in, const hit_record &rec, ray &scattered) const override{
        return 1;
    }
	virtual bool scatter(const ray &ray_in, const hit_record &rec, vec::vec3 &albedo, ray &scattered,float &pdf) const override
    {
        return false;
    }
    virtual const material *get_class_type(void) const override
    {
        return this;
    }

    virtual vec::vec3 emitted(const ray& ray_in,const hit_record& rec) const override
    {
        if(vec::dot(rec.normal,ray_in.direction())<0.0)return emit->value(rec.u,rec.v,rec.point);
        return vec::vec3(0);
    }

    texture *emit;
};