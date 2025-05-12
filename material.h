#ifndef MATERIAL_H
#define MATERIAL_H

#include "rtmath.h"

/**
 * Material class needs to calculate the color of a pixel based on the material of the object 
 * at that point. Uses the Phong model. Needs to account for ambient, specular, and diffuse
 * components.
 * 
 * All hittable objects should have a material. 
 * 
 * How this fits into the rendering loop: 
 * After a ray hits something, we retrieve it's material, then compute the color based on 
 * Phong shading model. 
 */
class material {
    public:
        double diffuse_ref_coef; //Kd
        color diffuse_color; //Od

        double specular_ref_coef; //Ks
        color specular_highlight_color; //Os

        double ambient_ref_coef; //Ka
        double glossiness; //Kgls
        double reflection_factor; //ref

        color compute_color(const vec3& light_dir, const color& ambient_light, 
                            const color& light_color, const vec3& camera_view_dir,
                            const vec3& surface_normal) const {
            color final_color = ambient_component(ambient_light, light_color) 
                              + diffuse_component(light_dir, light_color, surface_normal)
                              + specular_component(light_dir, light_color, camera_view_dir,
                                                   surface_normal);
            return clamp(final_color);
        }

        color compute_shadow_color(const color& light_intensity, const color& ambient_light) {
            return ambient_component(light_intensity, ambient_light);
        }

    private:
        color ambient_component(const color& light_intensity, const color& light_color) const {
            //ambient reflexivity * diffuse color * ambient light intensity
            color ambient_component = ambient_ref_coef * diffuse_color * light_intensity * light_color;
            return clamp(ambient_component);
        }

        color diffuse_component(const vec3& light_dir, const color& light_color, 
                                const vec3& surface_normal) const {
            double diff = std::max(0.0, dot(surface_normal, light_dir));
            color diffuse_component = diffuse_ref_coef * diffuse_color 
                                     * light_color * diff; //* light_intensity
            return clamp(diffuse_component);
        }

        color specular_component(const vec3& light_dir, const color& light_color, 
                                 const vec3& camera_view_dir, const vec3& surface_normal) const {
            vec3 ref_vec = 2 * dot(surface_normal, light_dir) * surface_normal - light_dir;
            double spec_angle = std::max(0.0, dot(unit_vector(ref_vec), unit_vector(camera_view_dir)));
            double spec = std::pow(spec_angle, glossiness);
            color specular_component = specular_ref_coef * specular_highlight_color
                                      * light_color * spec; //* light_intensity
            return clamp(specular_component);
        }

        color clamp(color c) const {
            return color(
                std::max(0.0, std::min(c.x(), 1.0)),
                std::max(0.0, std::min(c.y(), 1.0)),
                std::max(0.0, std::min(c.z(), 1.0))
            );
        }

};

#endif