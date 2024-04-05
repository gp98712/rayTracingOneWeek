#include <iostream>
#include "ray.h"
#include "color.h"
#include "vec3.h"

color ray_color(const ray& r) {
	vec3 unit_direction = unit_vector(r.direction());
	auto a = 0.5 * (unit_direction.y() + 1.0); // 0.0 ~ 1.0 으로 정규화.
	return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
}

int main() {
	auto aspect_ratio = 16.0 / 9.0; // 1.77778
	int image_width = 400;
	int image_height = static_cast<int>(image_width / aspect_ratio);
	image_height = (image_height < 1 ) ? 1 : image_height;

	// Camera
	auto focal_length = 1.0; // 카메라 초점 거리.
	auto viewport_height = 2.0;
	auto viewport_width = viewport_height * (static_cast<double>(image_width) / image_height);
	auto camera_center = point3(0, 0, 0);

	// Calculate the vectors across the horizontal and down the vertical viewport edges.

	// u <- 가로 v <- 세로
	// 
	auto viewport_u = vec3(viewport_width, 0, 0);
	auto viewport_v = vec3(0, -viewport_height, 0);

	// Delta u, v 계산
	auto pixel_delta_u = viewport_u / image_width; // 델타 u는 양수
	auto pixel_delta_v = viewport_v / image_height; // 델타 v는 음수.


	/*
	* viewport 좌표계
	* 뷰포트 중앙 [0,0]
	* 오른쪽으로 갈수록 x, 아래로 갈수록 y 증가.
	* 
	* [-viewport_u / 2(음수임), -viewport_v/2 (양수임) ] x [viewport_u/2(양수임), viewport_v/2 (음수임)]
	* 
	*/
	auto viewport_upper_left = camera_center - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;

	/*
	* pixel[0][0]이 바로 viewport_upper_left에서 시작하는것이 아니다. 00은 delta 사이에 위치한다.
	* 멀티샘플링을 생각해보면 이해가 쉽다.
	*/
	auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

	std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

	for (int j = 0; j < image_height; ++j) {
		std::clog << "\rScanlines remaining: " << (image_height - j) << " " << std::flush;

		for (int i = 0; i < image_width; ++i) {
			auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
			auto ray_direction = pixel_center - camera_center;
			ray r(camera_center, ray_direction);

			color pixel_color = ray_color(r);
			write_color(std::cout, pixel_color);
		}
	}

	std::clog << "\rDone.\n";

}