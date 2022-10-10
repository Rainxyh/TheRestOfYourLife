#include <fstream>
#include <iomanip>
#include <ctime>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "scene.cpp"
#include "pdf.h"

clock_t START_TIME, END_TIME;

vec::vec3 get_color(const ray &ray_, hitable *world, hitable *light_space, int depth)
{
	hit_record hrec;
	// superclass call implemented superclass virtual methods
	if (world->hit(ray_, 0.001, FLT_MAX, hrec)) // hitable_list or bvh_node instance, calls its overridden virtual method
	{
		vec::vec3 emmited = hrec.mat_ptr->emitted(ray_, hrec);
		scatter_record srec;
		if (depth < 50 && hrec.mat_ptr->scatter(ray_, hrec, srec))
		{
			if (srec.is_specular)
				return srec.attenuation * get_color(srec.specular_ray, world, light_space, depth + 1);

			hitable_pdf* hp=new hitable_pdf(light_space, hrec.point);
			cosine_pdf* cp=dynamic_cast<cosine_pdf*>(srec.pdf_ptr);
			mixture_pdf* p= new mixture_pdf(hp, cp);
			ray scattered = ray(hrec.point, p->generate(), ray_.get_time()); 
			float pdf_val = p->value(scattered.direction());
			delete srec.pdf_ptr,hp,cp,p; // memory leak
			return emmited + srec.attenuation * hrec.mat_ptr->scattering_pdf(ray_, hrec, scattered) * get_color(scattered, world, light_space, depth + 1) / pdf_val;
		}
		else
			return emmited;
	}
	else
		return vec::vec3(0);
}

#pragma region old_get_color
vec::vec3 get_color(const ray &ray_, hitable *world, int depth)
{
	hit_record rec;
	// superclass call implemented superclass virtual methods
	if (world->hit(ray_, 0.001, FLT_MAX, rec)) // hitable_list or bvh_node instance, calls its overridden virtual method
	{
		ray scattered;
		vec::vec3 attenuation;
		vec::vec3 albedo;
		vec::vec3 emmited = rec.mat_ptr->emitted(ray_, rec);
		float pdf_val;
		if (depth < 50 && rec.mat_ptr->scatter(ray_, rec, albedo, scattered, pdf_val))
		{
			//  Sampling Lights Directly
			// vec::vec3 on_light=vec::vec3(213+rand_float()*(343-213),554,227+rand_float()*(332-227));
			// vec::vec3 to_light = on_light-rec.point;
			// float distance_squared = to_light.squared_length();
			// to_light.make_unit_vector();
			// if(dot(to_light,rec.normal)<0)return emmited;
			// float light_area=(343-213)*(332-227);
			// float light_cosine=fabs(to_light.y());
			// if(light_cosine<0.000001)return emmited;
			// scattered = ray(rec.point,to_light,ray_.get_time());
			// pdf_val=distance_squared/(light_cosine*light_area);

			// pdf related to sampling the light
			// hitable *light_space = new xz_rect(213, 227, 343, 332, 554, 0);
			// hitable_pdf p(light_space, rec.point);

			// pdf related to cosine theta
			cosine_pdf p(rec.normal);

			// Mixture Densities
			// hitable *light_space = new xz_rect(213, 227, 343, 332, 554, 0);
			// hitable_pdf p0(light_space, rec.point);
			// cosine_pdf p1(rec.normal);
			// mixture_pdf p(&p0, &p1);

			scattered = ray(rec.point, p.generate(), ray_.get_time());
			pdf_val = p.value(scattered.direction());
			return emmited + albedo * rec.mat_ptr->scattering_pdf(ray_, rec, scattered) * get_color(scattered, world, depth + 1) / pdf_val;
		}
		else
			return emmited;
	}
	else
	{
		// vec::vec3 unit_direction = vec::unit_vector(ray_.direction());
		// float t = 0.5 * (unit_direction.y() + 1.0);
		// return (1.0 - t) * vec::vec3(1, 1, 1) + t * vec::vec3(0.5, 0.7, 1.0);
		return vec::vec3(0);
	}
}
#pragma endregion

void showProgress(int num, int sum)
{
	// std::cout << "\n";
	std::cout << "\r";
	std::cout << "running: " << (sum - num) * 100 / sum << "%";
}

hitable *fun(camera &cam, std::string &fig_name, hitable *(*func_ptr)(camera &cam, std::string &fig_name))
{
	return func_ptr(cam, fig_name);
}

int main(int argc, char *argv[])
{
	START_TIME = clock();

	std::string fig_name;
	camera camera;
	// hitable *world = TheNextWeek(camera,fig_name);
	hitable *world = fun(camera, fig_name, cornell_box);
	hitable *light_space = new xz_rect(213, 227, 343, 332, 554, 0);
	hitable *glass_sphere = new sphere(vec::vec3(190, 90, 190), 90, 0);
	hitable* a[2];
	a[0]=light_space;
	a[1]=glass_sphere;
	hitable* hlist = new hitable_list(a, 2);

	std::string file_path = "./output_fig/";
	if (0 != access(file_path.c_str(), 0))
	{
		if (mkdir(file_path.c_str(), S_IRUSR | S_IWUSR | S_IXUSR | S_IRWXG | S_IRWXO))
		{
			std::cerr << "make dir error" << std::endl;
		}
	}
	std::ofstream outfile;
	outfile.open(file_path + fig_name + ".ppm");
	int nx = 800;
	int ny = 600;
	int ns = 100;
	outfile << "P3\n"
			<< nx << " " << ny << "\n255" << std::endl;

	for (int j = ny; j >= 0; --j)
	{
		showProgress(j, ny);
		for (int i = 0; i < nx; ++i)
		{
			vec::vec3 color(0, 0, 0);
			for (int s = 0; s < ns; ++s) // every pixel random generate ray
			{
				float u = ((float)i + rand_float()) / nx, v = ((float)j + rand_float()) / ny;
				ray ray = camera.get_ray(u, v); // ray's time initial wrong to 0, lead to
				
				color += de_nan(get_color(ray, world, hlist, 1));
				// color += get_color(ray, world, hlist, 1);
				// color += get_color(ray, world, 1);
			}
			color /= (float)ns;
			color = gamma_correct(color);
			int ir = int(255.99f * color.r());
			int ig = int(255.99f * color.g());
			int ib = int(255.99f * color.b());
			outfile << ir << " " << ig << " " << ib << std::endl;
		}
	}

	END_TIME = clock();
	double delta_time = (double)(END_TIME - START_TIME) / CLOCKS_PER_SEC;
	std::cout << "Total time " << delta_time << " s" << std::endl;
	return 0;
}