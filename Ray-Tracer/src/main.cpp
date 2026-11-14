#include <iostream>
#include <print>

#include "util.hpp"
#include "hittable_list.hpp"
#include "sphere.hpp"
#include "quad.hpp"
#include "material.hpp"
#include "camera.hpp"
#include "bvh.hpp"
#include "texture.hpp"
#include "timer.hpp"

using namespace rt;

static std::tuple<HittableList, Camera> bouncing_spheres() {
	// Setup Camera
	constexpr CameraConfig config {
		.fov = 20.0,
		.defocus_angle = 0.6,
		.width = 1200,
		.samples_per_pixel = 500,
		.background = Color(0.7, 0.8, 1.0),
	};
	Camera camera(config);
	camera.look_from(Point3(13, 2, 3));
	camera.look_at(Point3(0, 0, 0));
	camera.set_focus_distance(10.0);
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
	constexpr CameraConfig config {
		.aspect_ratio = 16.0 / 9.0,
		.fov = 20.0,
		.width = 400,
		.max_depth = 50,
		.samples_per_pixel = 100,
		.background = Color(0.7, 0.8, 1.0),
	};
	Camera camera(config);
	camera.look_from(Point3(14, 2, 3));
	camera.look_at(Point3(0, 0, 0));
	camera.set_focus_distance(10.0);
	camera.initialize();

	// Build Scene
	HittableList world;
	auto checker = std::make_shared<CheckerTexture>(0.32, Color(0.2, 0.3, 0.1), Color(0.9, 0.9, 0.9));
	world.add(std::make_shared<Sphere>(Point3(0, -10, 0), 10, std::make_shared<Lambertian>(checker)));
	world.add(std::make_shared<Sphere>(Point3(0, 10, 0), 10, std::make_shared<Lambertian>(checker)));
	return { std::move(world), std::move(camera) };
}

static std::tuple<HittableList, Camera> earth() {
	// Setup camera
	constexpr CameraConfig config {
		.aspect_ratio = 16.0 / 9.0,
		.fov = 20.0,
		.width = 400,
		.max_depth = 50,
		.samples_per_pixel = 100,
		.background = Color(0.7, 0.8, 1.0),
	};
	Camera camera(config);
	camera.look_from(Point3(0, 0, 12));
	camera.look_at(Point3(0, 0, 0));
	camera.set_focus_distance(10.0);
	camera.initialize();
	
	// Create scene
    auto earth_texture = std::make_shared<ImageTexture>("earthmap.jpg");
    auto earth_surface = std::make_shared<Lambertian>(earth_texture);
    auto globe = std::make_shared<Sphere>(Point3(0,0,0), 2, earth_surface);

	return { HittableList(globe), std::move(camera) };
}

static std::tuple<HittableList, Camera> perlin_spheres() {
	constexpr CameraConfig config {
		.aspect_ratio = 16.0 / 9.0,
		.fov = 20.0,
		.background = Color(0.7, 0.8, 1.0),
	};
	Camera camera(config);
	camera.look_from(Point3(13, 2, 3));
	camera.look_at(Point3(0, 0, 0));
	camera.set_focus_distance(10.0);
	camera.initialize();

	// Create scene
	auto perlin_texture = std::make_shared<NoiseTexture>(4);
	auto perlin_material = std::make_shared<Lambertian>(perlin_texture);
	auto world = HittableList();
	world.add(std::make_shared<Sphere>(Point3(0, 2, 0), 2.0, perlin_material));
	world.add(std::make_shared<Sphere>(Point3(0, -1000, 0), 1000, perlin_material));
	return { std::move(world), std::move(camera) };
}

static std::tuple<HittableList, Camera> quads() {
	constexpr CameraConfig config {
		.aspect_ratio = 1.0,
		.fov = 80.0,
		.background = Color(0.7, 0.8, 1.0),
	};
	Camera camera(config);
	camera.look_from(Point3(0, 0, 9));
	camera.look_at(Point3(0, 0, 0));
	camera.set_focus_distance(10.0);
	camera.initialize();

	// Materials
	auto left_red     = std::make_shared<Lambertian>(Color(1.0, 0.2, 0.2));
	auto back_green   = std::make_shared<Lambertian>(Color(0.2, 1.0, 0.2));
	auto right_blue   = std::make_shared<Lambertian>(Color(0.2, 0.2, 1.0));
	auto upper_orange = std::make_shared<Lambertian>(Color(1.0, 0.5, 0.0));
	auto lower_teal   = std::make_shared<Lambertian>(Color(0.2, 0.8, 0.8));

	// Quads
	HittableList world;
	world.add(make_shared<Quad>(Point3(-3,-2, 5), Vec3(0, 0,-4), Vec3(0, 4, 0), left_red));
	world.add(make_shared<Quad>(Point3(-2,-2, 0), Vec3(4, 0, 0), Vec3(0, 4, 0), back_green));
	world.add(make_shared<Quad>(Point3( 3,-2, 1), Vec3(0, 0, 4), Vec3(0, 4, 0), right_blue));
	world.add(make_shared<Quad>(Point3(-2, 3, 1), Vec3(4, 0, 0), Vec3(0, 0, 4), upper_orange));
	world.add(make_shared<Quad>(Point3(-2,-3, 5), Vec3(4, 0, 0), Vec3(0, 0,-4), lower_teal));
	return { std::move(world), std::move(camera) };
}

static std::tuple<HittableList, Camera> simple_light() {
	HittableList world;
	const auto perlinTexture = std::make_shared<NoiseTexture>(4);
	world.add(std::make_shared<Sphere>(Point3(0,-1000,0), 1000, std::make_shared<Lambertian>(perlinTexture)));
	world.add(std::make_shared<Sphere>(Point3(0,2,0), 2, make_shared<Lambertian>(perlinTexture)));

	auto diffuseLight = std::make_shared<DiffuseLight>(Color(4,4,4));
	world.add(std::make_shared<Quad>(Point3(3,1,-2), Vec3(2,0,0), Vec3(0,2,0), diffuseLight));
	world.add(std::make_shared<Sphere>(Point3(0,7,0), 2, diffuseLight));

	constexpr CameraConfig config {
		.fov = 20.0,
		.background = Color(0,0,0),
	};
	Camera camera(config);
	camera.look_from(Point3(26, 3, 6));
	camera.look_at(Point3{0, 2, 0});
	camera.initialize();

	return { std::move(world), std::move(camera) };
}

static std::tuple<HittableList, Camera> cornell_box() {
	HittableList world;

	auto red = std::make_shared<Lambertian>(Color(.65, .05, .05));
	auto white = std::make_shared<Lambertian>(Color(.73, .73, .73));
	auto green = std::make_shared<Lambertian>(Color(.12, .45, .15));
	auto light = std::make_shared<DiffuseLight>(Color(15, 15, 15));

	world.add(std::make_shared<Quad>(Point3(555,0,0), Vec3(0,555,0), Vec3(0,0,555), green));
	world.add(std::make_shared<Quad>(Point3(0,0,0), Vec3(0,555,0), Vec3(0,0,555), red));
	world.add(std::make_shared<Quad>(Point3(343, 554, 332), Vec3(-130,0,0), Vec3(0,0,-105), light));
	world.add(std::make_shared<Quad>(Point3(0,0,0), Vec3(555,0,0), Vec3(0,0,555), white));
	world.add(std::make_shared<Quad>(Point3(555,555,555), Vec3(-555,0,0), Vec3(0,0,-555), white));
	world.add(std::make_shared<Quad>(Point3(0,0,555), Vec3(555,0,0), Vec3(0,555,0), white));

	std::shared_ptr<Hittable> box1 = box(Point3(0,0,0), Point3(165,330,165), white);
	// box1 = make_shared<RotateY>(box1, 15);
	box1 = make_shared<Translate>(box1, Vec3(265,0,295));
	world.add(box1);

	std::shared_ptr<Hittable> box2 = box(Point3(0,0,0), Point3(165,165,165), white);
	// box2 = make_shared<RotateY>(box2, -18);
	box2 = make_shared<Translate>(box2, Vec3(130,0,65));
	world.add(box2);

	CameraConfig config {
		.aspect_ratio = 1.0,
		.fov = 40,
		.width = 600,
		.samples_per_pixel = 5000,
	};
	Camera camera(config);
	camera.look_from(Point3(278, 278, -800));
	camera.look_at(Point3(278, 278, 0));
	camera.initialize();
	return { std::move(world), std::move(camera) };
}

void render_scene(std::tuple<HittableList, Camera> scene, const std::string& filename) {
	std::print("Scene: {}\n", filename);
	auto& [world, camera] = scene;
	Timer<std::chrono::seconds> timer;
	camera.render(BVHNode(world));
	timer.stop();
	write_to_file(camera.get_image_data(filename));
}

int main(const int argc, char* argv[]) {
	try {
		const int render = (argc == 2) ? std::stoi(argv[1]) : 0;
		switch (render) {
		case(0): {
			render_scene(bouncing_spheres(), "bouncing-spheres.ppm");
			break;
		}
		case(1): {
			render_scene(checkered_spheres(), "checkered-spheres.ppm");
			break;
		}
		case(2): {
			render_scene(earth(), "earth.ppm");
			break;
		}
		case(3): {
			render_scene(perlin_spheres(), "perlin-spheres.ppm");
			break;
		}
		case(4): {
			render_scene(quads(), "quads.ppm");
			break;
		}
		case(5): {
			render_scene(simple_light(), "simple-light.ppm");
			break;
		}
		case(6): {
			render_scene(cornell_box(), "cornell-box.ppm");
			break;
		}
		default:
			std::print( "No such scene\n");
		}
		
		#ifdef DNDEBUG
		std::print("\nPress enter to close > ");
		std::ignore = std::getchar();
		#endif
		
		return EXIT_SUCCESS;
	} catch (const std::exception& e) {
		std::print(std::cerr, "Failed to render scene: {}\n", e.what());
		return EXIT_FAILURE;
	}
}