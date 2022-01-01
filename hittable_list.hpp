#pragma once

#include <memory>
#include <vector>

#include "hittable.hpp"

struct hittable_list : hittable {
	std::vector<std::shared_ptr<hittable>> objects;

	hittable_list() = default;
	hittable_list(std::vector<std::shared_ptr<hittable>> objects) : objects{std::move(objects)} {}

	virtual bool test_hit(const ray& r, double t_min, double t_max, hit& info) const override {
		hit tmp_info;
		bool hit_anything = false;
		auto closest_so_far = t_max;

		for (const auto& object : objects) {
			if (object->test_hit(r, t_min, closest_so_far, tmp_info)) {
				hit_anything = true;
				closest_so_far = tmp_info.parameter;
				info = tmp_info;
			}
		}

		return hit_anything;
	}
};
