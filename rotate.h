#pragma once
#include "hitable.h"

class rotate_y : public hitable
{
public:
    rotate_y(){};
    rotate_y(hitable *hit_ptr, float angle);

    virtual bool hit(const ray &_ray, float t_min, float t_max, hit_record &rec) const override
    {
        vec::vec3 origin = _ray.origin();
        vec::vec3 direction = _ray.direction();
        origin[0] = cos_theta * _ray.origin()[0] - sin_theta * _ray.origin()[2];
        origin[2] = sin_theta * _ray.origin()[0] + cos_theta * _ray.origin()[2];
        direction[0] = cos_theta * _ray.direction()[0] - sin_theta * _ray.direction()[2];
        direction[2] = sin_theta * _ray.direction()[0] + cos_theta * _ray.direction()[2];
        ray rotate_r(origin, direction, _ray.get_time());
        if (hit_ptr->hit(rotate_r, t_min, t_max, rec))
        {
            vec::vec3 point = rec.point;
            vec::vec3 normal = rec.normal;
            point[0] = cos_theta * rec.point[0] + sin_theta * rec.point[2];
            point[2] = -sin_theta * rec.point[0] + cos_theta * rec.point[2];
            normal[0] = cos_theta * rec.normal[0] + sin_theta * rec.normal[2];
            normal[2] = -sin_theta * rec.normal[0] + cos_theta * rec.normal[2];
            rec.point = point;
            rec.normal = normal;
            return true;
        }
        return false;
    }

    virtual bool bounding_box(float t0, float t1, aabb &box) const override
    {
        box = bbox;
        return hasbox;
    }

    hitable *hit_ptr;
    float sin_theta;
    float cos_theta;
    bool hasbox;
    aabb bbox;
};

rotate_y::rotate_y(hitable *hit_ptr, float angle) : hit_ptr(hit_ptr)
{
    float radians = (M_PI / 180.) * angle;
    sin_theta = sin(radians);
    cos_theta = cos(radians);
    hasbox = hit_ptr->bounding_box(0, 1, bbox);
    vec::vec3 min(FLT_MAX, FLT_MAX, FLT_MAX);
    vec::vec3 max(-FLT_MAX, -FLT_MAX, -FLT_MAX);
    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 2; ++j)
            for (int k = 0; k < 2; ++k)
            {
                float x = i * bbox.max().x() + (1 - i) * bbox.min().x();
                float y = j * bbox.max().y() + (1 - j) * bbox.min().y();
                float z = k * bbox.max().z() + (1 - k) * bbox.min().z();
                float newx = cos_theta * x + sin_theta * z;
                float newz = -sin_theta * x + cos_theta * z;
                vec::vec3 tester(newx, y, newz);
                for (int c = 0; c < 3; ++c)
                {
                    if (tester[c] > max[c])
                        max[c] = tester[c];
                    if (tester[c] < min[c])
                        min[c] = tester[c];
                }
            }
    bbox = aabb(min, max);
}