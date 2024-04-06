#pragma once
#include "ray.h"

class hit_record {
public:
	point3 p;
	vec3 normal;
	double t;
	bool front_face;

	void set_face_normal(const ray& r, const vec3& outward_normal) {
		/*
		* ������ ���� ������, ������ ������ �ٸ��ٴ� �ǹ������� 
		* front-face
		*/
		front_face = dot(r.direction(), outward_normal) < 0;
		/*
		* �� å�� ������ ��ȣ���� ���� �̷��� �����Ǿ���.
		* ������ normal���� �ٲ��� �����
		* hit_record�� normal�� �׻� �Ի籤��(r)�� �ݴ�������� ���ϵ��� ������.
		* ��� ��ü�� front_face���� �ƴ����� front_face������ �Ǻ��Ѵ�.
		*/
		normal = front_face ? outward_normal : -outward_normal;
	}
};

class hittable {
public:
	virtual ~hittable() = default;

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};
