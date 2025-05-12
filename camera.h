#ifndef CAMERA_H
#define CAMERA_H

#include <fstream>

#include "hittable.h"
#include "hittable_list.h"

class camera {
    public:
        double aspect_ratio = 1.0;  // Ratio of image width over height
        int    image_width  = 500;  // Rendered image width in pixel count
        point3 look_from;           // Camera position
        point3 look_at;             // Camera target
        vec3 look_up;               // 'up' direction
        double vfov;                // vertical field of view (degrees)

        void render(const hittable_list& world, const std::string& filename) {
            std::ofstream output_file(filename, std::ios::out | std::ios::trunc);

            if (!output_file) {
                std::cerr <<"Error: could not open file " << filename << " to write.\n";
                return;
            }

            initialize();

            output_file << "P3\n" << image_width << ' ' << image_height << "\n255\n";

            for (int j = 0; j < image_height; j++) {
                std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
                for (int i = 0; i < image_width; i++) {
                    auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
                    auto ray_direction = pixel_center - look_from;
                    ray r(look_from, ray_direction);

                    color pixel_color = ray_color(r, world);
                    write_color(output_file, pixel_color);
                }
            }

            std::clog << "\rDone.                 \n";
            output_file.close();
        }

    private:
        int    image_height;   // Rendered image height
        point3 pixel00_loc;    // Location of pixel [0, 0]
        vec3   pixel_delta_u;  // Offset to pixel to the right
        vec3   pixel_delta_v;  // Offset to pixel below

        void initialize() {
            image_height = int(image_width / aspect_ratio);
            image_height = (image_height < 1) ? 1 : image_height;

            /** Ensure valid directions */
            if (fabs(dot(unit_vector(look_up), unit_vector(look_from - look_at))) > 0.9999) {
                std::cerr << "Warning: Look Up is almost parallel to Look From - Look At, readjusting.\n";
                look_up = vec3(0, 1, 0);                // This should be a safe vector... right?
            }

            vec3 w = unit_vector(look_from - look_at);  // Camera looking direction vector
            vec3 u = unit_vector(cross(look_up, w));    // Camera's right-hand direction vector
            vec3 v = cross(w, u);                       // The true up direction vector

            /** Determine viewport dimensions. */ 
            auto focal_length = 1.0;
            double theta = degrees_to_radians(vfov);
            double h = tan(theta / 2);                  // Half viewport height in world units
            double viewport_height = 2.0 * h;
            auto viewport_width = viewport_height * (double(image_width)/image_height);

            /** Calculate the vectors across the horizontal and down the vertical viewport edges. */ 
            auto viewport_u = viewport_width * u;
            auto viewport_v = -viewport_height * v;

            /** Calculate the horizontal and vertical delta vectors from pixel to pixel. */ 
            pixel_delta_u = viewport_u / image_width;
            pixel_delta_v = viewport_v / image_height;

            /** Calculate the location of the upper left pixel. */ 
            point3 viewport_upper_left =
                look_from - (focal_length * w) - viewport_u / 2 - viewport_v / 2;
            pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
        }

        color ray_color(const ray& r, const hittable_list& world) const {
            hit_record rec;
            ray current_ray = r;
            int max_depth = 3;  // Maximum reflections
            const double epsilon = 1e-8; // Minimum reflection contribution

            /** Apparently the bias needed to prevent 'shadow acne', 
             * which can happen due to floating point errors that cause 
             * a reflected ray to hit the same surface twice */
            const double bias = 1e-4;
            
            color final_color = color(0, 0, 0); // Accumulated color
            double reflection_factor = 1.0; // Initialize reflection at full strength

            /** Do this in a loop for multiple reflections, since recursion is slow */
            for (int i = 0; i < max_depth; i++) {
                if (world.hit(current_ray, interval(0, infinity), rec)) {
                    color local_color = color(0, 0, 0); // Reset local color each bounce

                    if (world.is_shadowed(rec.p, world.get_light_direction())) {
                        // In shadow, so only calculate ambient lighting
                        local_color = rec.mat->compute_shadow_color(world.get_light_color(), world.get_ambient_light());
                    } else {
                        // Not in shadow, so calculate full Phong shading
                        local_color = rec.mat->compute_color(
                            world.get_light_direction(),
                            world.get_ambient_light(),
                            world.get_light_color(),
                            unit_vector(-current_ray.direction()), // View direction
                            rec.normal
                        );
                    }

                    // Accumulate color
                    final_color += reflection_factor * local_color;

                    // Compute reflection
                    auto reflect_dir = reflect(current_ray.direction(), rec.normal);
                    current_ray = ray(rec.p + bias * rec.normal, reflect_dir);

                    // Update reflection factor after every bounce (reduces with each bounce)
                    reflection_factor *= rec.mat->reflection_factor;
                    
                    // Stop if reflections are insignificant
                    if (reflection_factor < epsilon) break;

                } else {
                    // If no hits, just background color contribution
                    final_color += reflection_factor * world.get_background_color();
                    break;
                }
            }

            return final_color;
        }
};

#endif