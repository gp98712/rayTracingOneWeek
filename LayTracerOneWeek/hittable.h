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
		* 내적의 값이 음수면, 벡터의 방향이 다르다는 의미임으로 
		* front-face
		*/
		front_face = dot(r.direction(), outward_normal) < 0;
		/*
		* 이 책의 저자의 선호도로 인해 이렇게 결정되었음.
		* 원래는 normal값이 바뀌지 않즈민
		* hit_record의 normal은 항상 입사광선(r)고 반대방향으로 향하도록 설정됨.
		* 대신 물체가 front_face인지 아닌지는 front_face값으로 판별한다.
		*/
		normal = front_face ? outward_normal : -outward_normal;
	}
};

class hittable {
public:
	virtual ~hittable() = default;

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};
