#pragma once
#include <random>
#include "vec3.h"

std::random_device rd;
std::default_random_engine random_engine(rd());
std::uniform_real_distribution<float> distribution(0, 1);

float rand_float()
{
	return distribution(random_engine);
}

float square_rand_float()
{
	return distribution(random_engine);
}

inline vec::vec3 random_to_sphere(float radius, float distance_squared)
{
	float r1 = rand_float();
	float r2 = rand_float();
	float z = 1 + r2 * (sqrt(1 - radius * radius / distance_squared) - 1);
	float phi = 2 * M_PI * r1;
	float x = cos(phi) * sqrt(1 - z * z);
	float y = sin(phi) * sqrt(1 - z * z);
	return vec::vec3(x, y, z);
}

vec::vec3 random_cosine_direction()
{
	float r1 = rand_float();
	float r2 = rand_float();
	float z = sqrt(1 - r2);
	float phi = 2 * M_PI * r1;
	float x = cos(phi) * sqrt(r2);
	float y = sin(phi) * sqrt(r2);
	return vec::vec3(x, y, z);
}

vec::vec3 random_in_unit_sphere()
{
	vec::vec3 p;
	do
	{
		p = 2 * vec::vec3(rand_float(), rand_float(), rand_float()) - vec::vec3(1, 1, 1);
	} while (p.length() >= 1.0);
	// return p;
	return vec::unit_vector(p);
}

vec::vec3 random_in_unit_disk()
{
	vec::vec3 p;
	do
	{
		p = 2 * vec::vec3(rand_float(), rand_float(), 0) - vec::vec3(1, 1, 0);
	} while (p.length() >= 1.0);
	return p;
}