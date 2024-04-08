#pragma once
#include "hittable.h"
#include "vec3.h"

class sphere : public hittable {
public:
    sphere(const point3& center, double radius, shared_ptr<material> mat)
        : center(center), radius(fmax(0, radius)), mat(mat) {}

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        vec3 oc = r.origin() - center;
        auto a = r.direction().length_squared();
        auto half_b = dot(oc, r.direction());
        auto c = oc.length_squared() - radius * radius;

        auto discriminant = half_b * half_b - a * c;
        if (discriminant < 0) return false;
        auto sqrtd = sqrt(discriminant);

        // root <- ray 교점, 첫번째 root <- 가장 가까운 교점
        auto root = (-half_b - sqrtd) / a;

        // 첫번째 교차점이 유효하지 않으면 2번째 교차점을 찾는다..
        /*
        * 이 부분은 혼란이 가중될 수 있으니 주석을 남기겠다.
        * 
        * 만약 (0,0,0) 반지름이 1인구가있고
        * 광선의 원점은 (0,0,0) 방향은 (1,0,0)이다.
        * 
        * 이경우 광선은 구의 중심에서 시작해서 구의 내부를 통과한다.
        * 
        * 이때 -sqrtd로 계산한 값에서 root는 0이될것이다. (원점 출발 충돌)
        * 
        * 때문에 root가 faild가 나고 2번째 교차점을 찾는 로직을 실행시킨다.
        */
        if (!ray_t.surrounds(root)) {
            root = (-half_b + sqrtd) / a;
            if (!ray_t.surrounds(root))
                return false;
        }

        rec.t = root;
        rec.p = r.at(rec.t);
        vec3 outward_normal = (rec.p - center) / radius;
        rec.set_face_normal(r, outward_normal);
        rec.mat = mat;

        return true;
	}

private:
	point3 center;
	double radius;
    shared_ptr<material> mat;
};
