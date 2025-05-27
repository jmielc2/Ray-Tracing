#pragma once

#include "util.hpp"
#include "hittable.hpp"
#include "material.hpp"
#include <array>
#include <thread>
#include <atomic>
#include <sstream>
#include <mutex>
#include <iomanip>

namespace rt {
	constexpr int num_threads = 10;

	class Camera {
	public:
		Point3 position, look_dir, vup;
		double aspect = 16.0 / 9.0, fov = degrees_to_radians(90.0), defocus_angle = 0, focus_dist = 10;
		int width = 400, samples_per_pixel = 100, max_depth = 50;

	private:
		struct RenderBatchInfo {
			RenderBatchInfo() = default;
			RenderBatchInfo(const Hittable& world);
			const Hittable& world;
		};

		Point3 viewport_pixel_00;
		Vec3 pixel_delta_u, pixel_delta_v;
		Vec3 u, v, w; // Camera basis vectors. u = right, v = forward, w = up
		Vec3 defocus_disk_u, defocus_disk_v;
		double viewport_height = 0.0, viewport_width = 0.0, pixel_sample_scale = 0.0;
		int height = 0;
		std::atomic<size_t> progress_counter = 0;
		std::vector<Color> pixel_buffer{ 0 };
		std::mutex ioMutex;

		Ray get_ray(size_t pixel_x, size_t pixel_y) const;
		Vec3 sample_square_offset() const;
		Vec3 sample_disk_offset() const;
		Vec3 defocus_disk_sample() const;
		Color ray_color(const Ray& ray, int depth, const Hittable& world) const;
		void update_basis_vectors();
		void update_disk_basis_vectors();
		void update_viewport_position();
		void update_viewport_position(const Vec3& viewport_u, const Vec3& viewport_v);
		void render_helper(const RenderBatchInfo& info);

	public:
		Camera();
		Camera(int width, double focus_dist, double defocus_angle, double aspect, double field_of_view);
		Camera(const Point3& eye, const Vec3& look_dir, const Vec3& vup, int width, double focus_dist, double defocus_angle, double aspect, double field_of_view);
		Camera(const Camera& other) = default;
		Camera(Camera&& other) = default;
		
		Camera& operator=(const Camera& other) = default;
		Camera& operator=(Camera&& other) = default;

		void initialize();
		void look_at(const Point3& look_at_point);
		void render(const Hittable& world);
		ImageData get_image_data(const std::string& filename) const;
	};
}

