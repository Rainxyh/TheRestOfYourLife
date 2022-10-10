#pragma once
#include <string>
#include "sphere.h"
#include "hitablelist.h"
#include "camera.h"
#include "lambertian.h"
#include "metal.h"
#include "dielectric.h"
#include "moving_sphere.h"
#include "bvh_node.h"
#include "constant_texture.h"
#include "checker_texture.h"
#include "noise_texture.h"
#include "image_texture.h"
#include "diffuse_light.h"
#include "rectangle.h"
#include "filp_normals.h"
#include "box.h"
#include "translate.h"
#include "rotate.h"
#include "constant_medium.h"
#include "isotropic.h"
#include "file.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

vec::vec3 vup = vec::vec3(0, 1, 0);
float vfov = 40;
float aspect = float(4) / float(3);
float aperture = 0.0;
float dist_to_focus = 10;
float time0 = 0.0, time1 = 1.0;

hitable *cornell_box(camera &cam, std::string &fig_name)
{
	fig_name = "cornell_box";

	vec::vec3 lookfrom = vec::vec3(278, 278, -800);
	vec::vec3 lookat = vec::vec3(278, 278, 0);
	cam = camera(lookfrom, lookat, vup, vfov, aspect, aperture, dist_to_focus, time0, time1);

	hitable **list = new hitable *[8];
	int i = 0;
	material *red = new lambertian(new constant_texture(vec::vec3(0.65, 0.05, 0.05)));
	material *white = new lambertian(new constant_texture(vec::vec3(0.73, 0.73, 0.73)));
	material *green = new lambertian(new constant_texture(vec::vec3(0.12, 0.45, 0.15)));
	material *blue = new lambertian(new constant_texture(vec::vec3(0.12, 0.15, 0.75)));
	material *yellow = new lambertian(new constant_texture(vec::vec3(0.93, 0.93, 0.15)));
	material *gray = new lambertian(new constant_texture(vec::vec3(0.3, 0.3, 0.3)));
	material *light = new diffuse_light(new constant_texture(vec::vec3(15, 15, 15)));
	list[i++] = new filp_normals(new yz_rect(0, 0, 555, 555, 555, green));	   // left
	list[i++] = new yz_rect(0, 0, 555, 555, 0, red);						   // right
	list[i++] = new filp_normals(new xz_rect(213, 227, 343, 332, 554, light)); // light
	list[i++] = new filp_normals(new xz_rect(0, 0, 555, 555, 555, yellow));	   // top
	list[i++] = new xz_rect(0, 0, 555, 555, 0, blue);						   // bottom
	list[i++] = new filp_normals(new xy_rect(0, 0, 555, 555, 555, white));	   // background
	// list[i++] = new box(vec::vec3(130, 0, 65), vec::vec3(295, 165, 230), white);
	// list[i++] = new box(vec::vec3(265, 0, 295), vec::vec3(430, 330, 460), white);
	// list[i++] = new translate(new rotate_y(new box(vec::vec3(0, 0, 0), vec::vec3(165, 165, 165), white), -18), vec::vec3(130, 0, 65));
	material* glass=new dielectric(1.5);
	list[i++] = new sphere(vec::vec3(190, 90, 190), 90, glass);
	list[i++] = new translate(new rotate_y(new box(vec::vec3(0, 0, 0), vec::vec3(165, 330, 165), white), 15), vec::vec3(265, 0, 295));
	material *aluminum = new metal(vec::vec3(0.8, 0.85, 0.88), 0.);
	//list[i++] = new translate(new rotate_y(new box(vec::vec3(0), vec::vec3(165, 330, 165), aluminum), 15), vec::vec3(265, 0, 295));
	return new hitable_list(list, i);
}

hitable *cornell_room(camera &cam, std::string &fig_name)
{
	fig_name = "cornell_room";

	vec::vec3 lookfrom = vec::vec3(278, 278, -800);
	vec::vec3 lookat = vec::vec3(278, 278, 0);
	cam = camera(lookfrom, lookat, vup, vfov, aspect, aperture, dist_to_focus, time0, time1);

	hitable **list = new hitable *[6];
	int i = 0;
	material *red = new lambertian(new constant_texture(vec::vec3(0.65, 0.05, 0.05)));
	material *white = new lambertian(new constant_texture(vec::vec3(0.73, 0.73, 0.73)));
	material *green = new lambertian(new constant_texture(vec::vec3(0.12, 0.45, 0.15)));
	material *blue = new lambertian(new constant_texture(vec::vec3(0.12, 0.15, 0.45)));
	material *yellow = new lambertian(new constant_texture(vec::vec3(0.93, 0.93, 0.15)));
	material *light = new diffuse_light(new constant_texture(vec::vec3(15, 15, 15)));
	list[i++] = new filp_normals(new yz_rect(0, 0, 555, 555, 555, green));	   // left
	list[i++] = new yz_rect(0, 0, 555, 555, 0, red);						   // right
	list[i++] = new filp_normals(new xz_rect(213, 227, 343, 332, 554, light)); // top light
	list[i++] = new filp_normals(new xz_rect(0, 0, 555, 555, 555, yellow));	   // top
	list[i++] = new xz_rect(0, 0, 555, 555, 0, blue);						   // bottom
	list[i++] = new filp_normals(new xy_rect(0, 0, 555, 555, 555, white));	   // background
	return new hitable_list(list, i);
}

hitable *simple_light(camera &cam, std::string &fig_name)
{
	fig_name = "simple_light";

	vec::vec3 lookfrom = vec::vec3(13, 2, 3);
	vec::vec3 lookat = vec::vec3(0, 0, 0);
	cam = camera(lookfrom, lookat, vup, vfov, aspect, aperture, dist_to_focus, time0, time1);

	texture *pertext = new noise_texture(4);
	hitable **list = new hitable *[4];
	list[0] = new sphere(vec::vec3(0, -1000, 0), 1000, new lambertian(pertext));
	list[1] = new sphere(vec::vec3(0, 2, 0), 2, new lambertian(pertext));
	list[2] = new sphere(vec::vec3(0, 7, 0), 2, new diffuse_light(new constant_texture(vec::vec3(4, 4, 4))));
	list[3] = new xy_rect(3, 1, 5, 3, -2, new diffuse_light(new constant_texture(vec::vec3(4, 4, 4))));
	return new hitable_list(list, 4);
}

hitable *earth(camera &cam, std::string &fig_name)
{
	fig_name = "earth";

	vec::vec3 lookfrom = vec::vec3(13, 2, 3);
	vec::vec3 lookat = vec::vec3(0, 0, 0);
	cam = camera(lookfrom, lookat, vup, vfov, aspect, aperture, dist_to_focus, time0, time1);

	int nx, ny, nn;
	unsigned char *tex_data = stbi_load("earth.png", &nx, &ny, &nn, 0);
	material *mat = new lambertian(new image_texture(tex_data, nx, ny, nn));
	return new sphere(vec::vec3(0, 0, 0), 2, mat);
}

hitable *two_perlin_spheres(camera &cam, std::string &fig_name)
{
	fig_name = "two_perlin_spheres";

	vec::vec3 lookfrom = vec::vec3(13, 2, 3);
	vec::vec3 lookat = vec::vec3(0, 0, 0);
	cam = camera(lookfrom, lookat, vup, vfov, aspect, aperture, dist_to_focus, time0, time1);

	hitable **list = new hitable *[2];
	float scale = 5;
	texture *p_texture = new noise_texture(scale);
	list[0] = new sphere(vec::vec3(0, -1000, 0), 1000, new lambertian(p_texture));
	list[1] = new sphere(vec::vec3(0, -2, 0), 2, new lambertian(p_texture));
	hitable *world = new hitable_list(list, 2);
	return world; // return point to hitable
}

hitable *two_spheres(camera &cam, std::string &fig_name)
{
	fig_name = "two_spheres";

	vec::vec3 lookfrom = vec::vec3(13, 2, 3);
	vec::vec3 lookat = vec::vec3(0, 0, 0);
	cam = camera(lookfrom, lookat, vup, vfov, aspect, aperture, dist_to_focus, time0, time1);

	hitable **list = new hitable *[2];
	texture *c_texture = new checker_texture(new constant_texture(vec::vec3(0.2, 0.3, 0.1)), new constant_texture(vec::vec3(0.9, 0.9, 0.9)));
	list[0] = new sphere(vec::vec3(0, -10, 0), 10, new lambertian(c_texture));
	list[1] = new sphere(vec::vec3(0, 10, 0), 10, new lambertian(c_texture));
	hitable *world = new hitable_list(list, 2); // two objects bvh slow than direct compute
	// hitable *world = new bvh_node(list, 2, 0.0, 1.0);
	return world; // return point to hitable
}

hitable *InOneWeekend(camera &cam, std::string &fig_name)
{
	fig_name = "InOneWeekend";

	vec::vec3 lookfrom = vec::vec3(13, 2, 3);
	vec::vec3 lookat = vec::vec3(0, 0, 0);
	cam = camera(lookfrom, lookat, vup, vfov, aspect, aperture, dist_to_focus, time0, time1);

	int n = 500;
	int count = 0;
	hitable **list = new hitable *[n + 1];

	list[count] = new sphere(vec::vec3(0, -1000, 0), 1000, new lambertian(new constant_texture(vec::vec3(0.5, 0.5, 0.5))));
	// list[count] = new sphere(vec::vec3(0, -1000, 0), 1000, new lambertian(new checker_texture(new constant_texture(vec::vec3(0.2, 0.3, 0.1)), new constant_texture(vec::vec3(0.9, 0.9, 0.9)))));
	for (int a = -10; a < 10; a++)
	{
		for (int b = -10; b < 10; b++)
		{
			float choose_mat = rand_float();
			vec::vec3 center(a + 0.9 * rand_float(), 0.2, b + 0.9 * rand_float());
			if ((center - vec::vec3(4, 0.2, 0)).length() > 0.9)
			{
				if (choose_mat < 0.8)
				{ // diffuse
					vec::vec3 moving_center = center + vec::vec3(0, 0.3 * rand_float(), 0);
					list[++count] = new moving_sphere(center, moving_center, 0.0, 1.0, 0.2, new lambertian(new constant_texture(vec::vec3(square_rand_float(), square_rand_float(), square_rand_float()))));
				}
				else if (choose_mat < 0.95)
				{ // metal
					list[++count] = new sphere(center, 0.2, new metal(0.5 * vec::vec3(1 + rand_float(), 1 + rand_float(), 1 + rand_float()), 0.5 * rand_float()));
				}
				else
				{ // glass
					list[++count] = new sphere(center, 0.2, new dielectric(1.5));
				}
			}
		}
	}
	list[++count] = new sphere(vec::vec3(0, 1, 0), 1.0, new dielectric(1.5));
	int nx, ny, nn;
	unsigned char *tex_data = stbi_load("earth.png", &nx, &ny, &nn, 0);
	material *mat = new lambertian(new image_texture(tex_data, nx, ny, nn));
	list[++count] = new sphere(vec::vec3(-4, 1, 0), 1.0, mat);
	// list[++count] = new sphere(vec::vec3(-4, 1, 0), 1.0, new lambertian(new constant_texture(vec::vec3(0.4, 0.2, 0.1))));
	list[++count] = new sphere(vec::vec3(4, 1, 0), 1.0, new metal(vec::vec3(0.7, 0.6, 0.5), 0.0));

	// superclass pointer points to child class reference
	// hitable *world = new hitable_list(list, count + 1); // hitable_list still is a hitable, but list contains sphere
	hitable *world = new bvh_node(list, count + 1, 0.0, 1.0);
	return world; // return point to hitable
}