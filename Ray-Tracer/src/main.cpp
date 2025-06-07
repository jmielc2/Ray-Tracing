#include "util.hpp"
#include "hittable_list.hpp"
#include "sphere.hpp"
#include "material.hpp"
#include "camera.hpp"
#include "bvh.hpp"
#include "texture.hpp"

using namespace rt;

static std::tuple<HittableList, Camera> bouncing_spheres() {
	// Setup Camera
	Camera camera(1200, 10.0, 0.6, 16.0 / 9.0, 20.0);
	camera.position = Point3(13, 2, 3);
	camera.look_at(Point3(0, 0, 0));
	camera.samples_per_pixel = 500;
	camera.max_depth = 50;
	camera.initialize();

	// Build Scene
	HittableList world;
	auto ground_texture = std::make_shared<CheckerTexture>(0.32, Color(0.2, 0.3, 0.1), Color(0.9, 0.9, 0.9));
	auto ground_material = std::make_shared<Lambertian>(ground_texture);
	world.add(std::make_shared<Sphere>(Point3(0, -1000, 0), 1000, ground_material));
	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			auto choose_mat = random_double();
			Point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

			if ((center - Point3(4, 0.2, 0)).length() > 0.9) {
				std::shared_ptr<Material> sphere_material;

				if (choose_mat < 0.8) {
					// diffuse
					auto albedo = Color::random() * Color::random();
					sphere_material = std::make_shared<Lambertian>(albedo);
					Point3 destination(center + Vec3(0, random_double(), 0) * 0.5);
					world.add(std::make_shared<Sphere>(center, destination, 0.2, sphere_material));
				} else if (choose_mat < 0.95) {
					// metal
					auto albedo = Color::random(0.5, 1);
					auto fuzz = random_double(0, 0.5);
					sphere_material = std::make_shared<Metal>(albedo, fuzz);
					world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
				} else {
					// glass
					sphere_material = std::make_shared<Dielectric>(1.5);
					world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
				}
			}
		}
	}
	const auto material1 = std::make_shared<Dielectric>(1.5);
	const auto material2 = std::make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
	const auto material3 = std::make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
	world.add(std::make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));
	world.add(std::make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));
	world.add(std::make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));
	return { std::move(world), std::move(camera) };
}

static std::tuple<HittableList, Camera> checkered_spheres() {
	// Setup Camera
	Camera camera(400, 10.0, 0.0, 16.0 / 9.0, 20.0);
	camera.position = Point3(13, 2, 3);
	camera.look_at(Point3(0, 0, 0));
	camera.samples_per_pixel = 100;
	camera.max_depth = 50;
	camera.initialize();

	// Build Scene
	HittableList world;
	auto checker = std::make_shared<CheckerTexture>(0.32, Color(0.2, 0.3, 0.1), Color(0.9, 0.9, 0.9));
	world.add(std::make_shared<Sphere>(Point3(0, -10, 0), 10, std::make_shared<Lambertian>(checker)));
	world.add(std::make_shared<Sphere>(Point3(0, 10, 0), 10, std::make_shared<Lambertian>(checker)));
	return { std::move(world), std::move(camera) };
}

static std::tuple<HittableList, Camera> earth() {
    auto earth_texture = std::make_shared<ImageTexture>("earthmap.jpg");
    auto earth_surface = std::make_shared<Lambertian>(earth_texture);
    auto globe = std::make_shared<Sphere>(Point3(0,0,0), 2, earth_surface);

    Camera camera(
		400, 10.0, 16.0 / 9.0, 0.0, 20.0
	);

    camera.samples_per_pixel = 100;
    camera.max_depth = 50;
	camera.initialize();

    camera.position = Point3(0,0,12);
    camera.look_at(Point3(0,0,0));
	return { HittableList(globe), std::move(camera) };
}

int main(int argc, char* argv[]) {
	try {
		// Build Scene
		// auto [world, camera] = bouncing_spheres();
		// auto [world, camera] = checkered_spheres();
		auto [world, camera] = earth();

		// Ray Trace
		{
			const Timer<std::chrono::seconds> timer;
			camera.render(BVHNode(world));
		}

		// Output Image
		const std::string filename = (argc == 2) ? argv[1] : "image.ppm";
		write_to_file(camera.get_image_data(filename));
		std::cout << "\nPress enter to close ";
		std::ignore = std::getchar();
		return EXIT_SUCCESS;
	} catch (const std::exception& e) {
		std::cerr << e.what() << "\n";
		return EXIT_FAILURE;
	}
}