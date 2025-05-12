#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "rtmath.h"

#include "hittable.h"
#include <vector>

class hittable_list : public hittable {
    public: 
        std::vector<shared_ptr<hittable>> objects;

        hittable_list() {}
        hittable_list(shared_ptr<hittable> object) { add(object); }

        void clear() { objects.clear(); }

        void add(shared_ptr<hittable> object) {
            objects.push_back(object);
        }

        bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
            hit_record temp_rec;
            bool hit_anything = false;
            auto closest_so_far = ray_t.max;

            for (const auto& object : objects) {
                if (object->hit(r, interval(ray_t.min, closest_so_far), temp_rec)) {
                    hit_anything = true;
                    closest_so_far = temp_rec.t;
                    rec = temp_rec;
                }
            }
            return hit_anything;
        }

        bool is_shadowed(const point3& p, const vec3& light_dir) const { 
            ray shadow_ray(p + light_dir * 1e-4, light_dir);
            hit_record shadow_hit;
            return hit(shadow_ray, interval(0.001, infinity), shadow_hit);  
        }
        
        const vec3& get_light_direction() const { return light_direction;}
        void set_light_direction(const vec3& light_direction) { 
            this->light_direction = unit_vector(light_direction); 
        }

        const color& get_light_color() const { return light_color; }
        void set_light_color(const color& light_color) { this->light_color = light_color; }

        const color& get_ambient_light() const { return ambient_light; }
        void set_ambient_light(const color& ambient_light) { this->ambient_light = ambient_light; }

        const color& get_background_color() const { return background_color; }
        void set_background_color(const color& background_color) { this->background_color = background_color; }

    private: 
        /** Values needed for calculating material shading */
        vec3 light_direction;
        color light_color;
        color ambient_light;
        color background_color;
        
};

#endif