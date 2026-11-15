#include <iomanip>
#include <thread>
#include <print>
#include <iostream>

#include "camera.hpp"
#include "material.hpp"

namespace rt {
	/*
	* Private Member Functions
	*/

	const size_t NUM_THREADS = (std::thread::hardware_concurrency() >= 2) ? std::thread::hardware_concurrency() / 2 : 1;

	Ray Camera::get_ray(const int pixel_x, const int pixel_y) const {
		const Vec3 offset = sample_square_offset();
		const Point3 pixel_viewport_loc = viewport_pixel_00 + ((pixel_x + offset.x()) * pixel_delta_u) + ((pixel_y + offset.y()) * pixel_delta_v);
		Point3 ray_origin = (defocus_angle <= 0) ? position : defocus_disk_sample();
		const double ray_time = random_double();
		return {ray_origin, pixel_viewport_loc - ray_origin, ray_time};
	}

	Vec3 Camera::sample_square_offset() {
		return {random_double() - 0.5, random_double() - 0.5, 0.0};
	}

	Vec3 Camera::sample_disk_offset() {
		const double angle = random_double(0.0, 2 * pi);
		return {std::cos(angle) * 0.5, std::sin(angle) * 0.5, 0.0};
	}

	Vec3 Camera::defocus_disk_sample() const {
		const Vec3 disk_offset = random_in_unit_disk();
		return (disk_offset.x() * defocus_disk_u) + (disk_offset.y() * defocus_disk_v) + position;
	}

	Color Camera::ray_color(const Ray &ray, size_t depth, const Hittable &world) {
		if (depth == 0) [[unlikely]] {
			return {0, 0, 0};
		}
		const auto hit_result = world.hit(ray, Interval{ 0.001, infinity });
		if (hit_result.has_value()) {
			const HitRecord& hit_record = hit_result.value();
			const auto scatter_result = hit_record.material->scatter(ray, hit_record);
			const auto color_from_emission = hit_record.material->emitted(hit_result->u, hit_result->v, hit_result->point);
			if (scatter_result.has_value()) {
				const ScatterRecord& scatter_record = scatter_result.value();
				const auto color_from_scatter = scatter_record.attenuation * ray_color(scatter_record.scatter_ray, depth - 1, world);
				return color_from_emission + color_from_scatter;
			}
			return color_from_emission;
		}
		return background;
	}

	void Camera::update_basis_vectors() {
		v = normalize(look_dir);
		u = normalize(cross(v, vup));
		w = cross(u, v);
		update_disk_basis_vectors();
	}

	void Camera::update_disk_basis_vectors() {
		const double defocus_radius = std::tan(degrees_to_radians(defocus_angle * 0.5)) * focus_dist;
		defocus_disk_u = defocus_radius * u;
		defocus_disk_v = defocus_radius * w;
	}

	void Camera::update_viewport_position() {
		const Vec3 viewport_u = u * viewport_width;
		const Vec3 viewport_v = -w * viewport_height;
		update_viewport_position(viewport_u, viewport_v);
	}

	void Camera::update_viewport_position(const Vec3& viewport_u, const Vec3& viewport_v) {
		const Point3 viewport_center = position + v * focus_dist;
		const Point3 viewport_upper_left = viewport_center - 0.5 * (viewport_u + viewport_v);
		viewport_pixel_00 = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
	}

	void Camera::render_helper(const RenderBatchInfo info) {
		auto curScanline = progress_counter->fetch_add(1);
		while (curScanline < height) [[likely]] {
			{
				const auto lock = lockIO();
				std::print("Progress: {:.2f}%\n", static_cast<double>(curScanline + 1) * 100.0 / static_cast<double>(height));
				std::cout.flush();
			}
			for (size_t x = 0; x < width; x++) {
				Color color(0, 0, 0);
				for (size_t i = 0; i < samples_per_pixel; i++) {
					Ray ray = get_ray(x, curScanline);
					color += ray_color(ray, max_depth, info.world);
				}
				pixel_buffer[curScanline * width + x] = color / samples_per_pixel;
			}
			curScanline = progress_counter->fetch_add(1);
		}
	}

	void Camera::initialize() {
		pixel_sample_scale = 1.0 / samples_per_pixel;
		height = static_cast<int>(width / aspect);
		height = (height > 0) ? height : 1;
		pixel_buffer.resize(width * height);
		update_basis_vectors();
		const double slope = std::tan(fov * 0.5);
		viewport_height = 2.0 * slope * focus_dist;
		viewport_width = viewport_height * (static_cast<double>(width) / height);
		const Vec3 viewport_u = u * viewport_width;
		const Vec3 viewport_v = -w * viewport_height;
		pixel_delta_u = viewport_u / static_cast<double>(width);
		pixel_delta_v = viewport_v / static_cast<double>(height);
		update_viewport_position(viewport_u, viewport_v);
	}

	/*
	* Public Member Functions
	*/

	Camera::Camera(const CameraConfig& config) :
		vup(config.vup),
		width(config.width),
		defocus_angle(config.defocus_angle),
		aspect(config.aspect_ratio),
		fov(degrees_to_radians(config.fov)),
		progress_counter(std::make_unique<std::atomic<size_t>>()),
		background(config.background)
	{
		initialize();
	}

	void Camera::look_from(const Point3& look_from_point) {
		position = look_from_point;
	}

	void Camera::look_at(const Point3& look_at_point) {
		look_dir = look_at_point - position;
		focus_dist = look_dir.length();
		look_dir = normalize(look_dir);
		update_basis_vectors();
		update_viewport_position();
	}

	void Camera::render(const Hittable& world) {
		std::print("Rendering on {} threads.\n", NUM_THREADS);
		progress_counter->store(0);
		const size_t thread_count = NUM_THREADS - 1;
		std::vector<std::jthread> threads(thread_count);
		for (size_t i = 0; i < thread_count; i++) {
			threads[i] = std::jthread(&Camera::render_helper, this, RenderBatchInfo{ world });
		}
		render_helper(RenderBatchInfo{ world });
	}

	ImageData Camera::get_image_data(const std::string& filename) const {
		return ImageData {
			.name = filename,
			.width = width, .height = height,
			.pixels = pixel_buffer
		};
	}
}
