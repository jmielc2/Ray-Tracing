#pragma once

#include "vec3.hpp"
#include "color.hpp"
#include "ray.hpp"
#include "hittable.hpp"

namespace rt {
	struct CameraConfig {
		Point3 position = Point3(0.0, 0.0, 10.0);
		Vec3 look_dir = Vec3(0.0, 0.0, -1.0);
		Vec3 vup = Vec3(0.0, 1.0, 0.0);
		double aspect_ratio = 16.0 / 9.0;
		double fov = degrees_to_radians(90.0);
		double defocus_angle = 0.0;
		double focus_dist = 10.0;
		int width = 400, max_depth = 50, samples_per_pixel = 100;
	};

	class Camera {
		struct RenderBatchInfo {
			const Hittable& world;
		};

		Point3 position, look_dir, vup;
		double aspect = 16.0 / 9.0, fov = degrees_to_radians(90.0), defocus_angle = 0, focus_dist = 10;
		int width = 400, max_depth = 50, samples_per_pixel = 100;
		Point3 viewport_pixel_00;
		Vec3 pixel_delta_u, pixel_delta_v;
		Vec3 u, v, w; // Camera basis vectors. u = right, v = forward, w = up
		Vec3 defocus_disk_u, defocus_disk_v;
		double viewport_height = 0.0, viewport_width = 0.0, pixel_sample_scale = 0.0;
		int height = 0;
		std::unique_ptr<std::atomic<size_t>> progress_counter;
		std::vector<Color> pixel_buffer{ 0 };

		[[nodiscard]] Ray get_ray(int pixel_x, int pixel_y) const;
		[[nodiscard]] static Vec3 sample_square_offset();
		[[nodiscard]] static Vec3 sample_disk_offset();
		[[nodiscard]] Vec3 defocus_disk_sample() const;
		[[nodiscard]] static Color ray_color(const Ray& ray, size_t depth, const Hittable& world);
		void update_basis_vectors();
		void update_disk_basis_vectors();
		void update_viewport_position();
		void update_viewport_position(const Vec3& viewport_u, const Vec3& viewport_v);
		void render_helper(RenderBatchInfo info);

	public:
		explicit Camera(const CameraConfig& config);
		Camera(Camera&&) noexcept = default;
		Camera(const Camera&) = delete;
		~Camera() = default;

		Camera& operator=(const Camera&) = delete;
		Camera& operator=(Camera&&) noexcept = default;

		void initialize();
		void look_at(const Point3& look_at_point);
		void render(const Hittable& world);
		[[nodiscard]] ImageData get_image_data(const std::string& filename) const;
	};
}

