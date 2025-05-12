#include "rtmath.h"

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "triangle.h"

/** Function to create a sphere */
void create_sphere(hittable_list& world, point3 position, 
                   double radius, color diffuse, color specular, 
                   double glossiness, double diffuse_coef, 
                   double specular_coef, double ambient_coef, 
                   double reflection_factor = 0.1) {
    auto mat = make_shared<material>();
    mat->diffuse_ref_coef = diffuse_coef;
    mat->specular_ref_coef = specular_coef;
    mat->ambient_ref_coef = ambient_coef;
    mat->diffuse_color = diffuse;
    mat->specular_highlight_color = specular;
    mat->glossiness = glossiness;
    mat->reflection_factor = reflection_factor;

    auto sphere_obj = make_shared<sphere>(position, radius, mat);
    world.add(sphere_obj);
}

void create_triangle(hittable_list& world, point3 a, point3 b, 
                     point3 c, color diffuse, color specular, 
                     double glossiness, double diffuse_coef, 
                     double specular_coef, double ambient_coef, 
                     double reflection_factor = 0) {
    auto mat = make_shared<material>();
    mat->diffuse_ref_coef = diffuse_coef;
    mat->specular_ref_coef = specular_coef;
    mat->ambient_ref_coef = ambient_coef;
    mat->diffuse_color = diffuse;
    mat->specular_highlight_color = specular;
    mat->glossiness = glossiness;
    mat->reflection_factor = reflection_factor;

    auto triangle_obj = make_shared<triangle>(a, b, c, mat);
    
    world.add(triangle_obj);
}

int main () {
    /** Image 1 */
    hittable_list world1;
    world1.set_light_direction(vec3(0.0, 1.0, 0.0));
    world1.set_light_color(color(1.0, 1.0, 1.0));
    world1.set_ambient_light(color(0.0, 0.0, 0.0));
    world1.set_background_color(color(0.2, 0.2, 0.2));

    auto sphere1_mat = make_shared<material>();
    sphere1_mat->diffuse_ref_coef = 0.7;
    sphere1_mat->specular_ref_coef = 0.1;
    sphere1_mat->ambient_ref_coef = 0.1;
    sphere1_mat->diffuse_color = color(1.0, 0.0, 1.0);
    sphere1_mat->specular_highlight_color = color(1.0, 1.0, 1.0);
    sphere1_mat->glossiness = 16.0;

    world1.add(make_shared<sphere>(point3(0, 0, 0), 0.4, sphere1_mat));

    camera cam1;
    cam1.aspect_ratio = 16.0 / 9.0;
    cam1.image_width = 400;
    cam1.look_at = point3(0, 0, 0);
    cam1.look_from = point3(0, 0, 1);
    cam1.look_up = vec3(0, 1, 0);
    cam1.vfov = 90;


    cam1.render(world1, "im1.ppm");

    /** Image 2 */
    hittable_list world2;
    world2.set_light_direction(vec3(1.0, 1.0, 1.0));
    world2.set_light_color(color(1.0, 1.0, 1.0));
    world2.set_ambient_light(color(0.1, 0.1, 0.1));
    world2.set_background_color(color(0.2, 0.2, 0.2));

    /** White Sphere */
    auto white_sphere_mat = make_shared<material>();
    white_sphere_mat->diffuse_ref_coef = 0.8;
    white_sphere_mat->specular_ref_coef = 0.1;
    white_sphere_mat->ambient_ref_coef = 0.3;
    white_sphere_mat->diffuse_color = color(1.0, 1.0, 1.0);
    white_sphere_mat->specular_highlight_color = color(1.0, 1.0, 1.0);
    white_sphere_mat->glossiness = 4.0;

    auto white_sphere = make_shared<sphere>(point3(0.45, 0.0, -0.15), 0.15, white_sphere_mat);
    world2.add(white_sphere);

    /** Red Sphere */
    auto red_sphere_mat = make_shared<material>();
    red_sphere_mat->diffuse_ref_coef = 0.6;
    red_sphere_mat->specular_ref_coef = 0.3;
    red_sphere_mat->ambient_ref_coef = 0.1;
    red_sphere_mat->diffuse_color = color(1.0, 0.0, 0.0);
    red_sphere_mat->specular_highlight_color = color(1.0, 1.0, 1.0);
    red_sphere_mat->glossiness = 32.0;

    auto red_sphere = make_shared<sphere>(point3(0.0, 0.0, -0.1), 0.2, red_sphere_mat);
    world2.add(red_sphere);

    /** Green Sphere */
    auto green_sphere_mat = make_shared<material>();
    green_sphere_mat->diffuse_ref_coef = 0.7;
    green_sphere_mat->specular_ref_coef = 0.2;
    green_sphere_mat->ambient_ref_coef = 0.1;
    green_sphere_mat->diffuse_color = color(0.0, 1.0, 0.0);
    green_sphere_mat->specular_highlight_color = color(0.5, 1.0, 0.5);
    green_sphere_mat->glossiness = 64.0;

    auto green_sphere = make_shared<sphere>(point3(-0.6, 0.0, 0.0), 0.3, green_sphere_mat);
    world2.add(green_sphere);

    /** Blue Sphere */
    auto blue_sphere_mat = make_shared<material>();
    blue_sphere_mat->diffuse_ref_coef = 0.9;
    blue_sphere_mat->specular_ref_coef = 0.0;
    blue_sphere_mat->ambient_ref_coef = 0.1;
    blue_sphere_mat->diffuse_color = color(0.0, 0.0, 1.0);
    blue_sphere_mat->specular_highlight_color = color(1.0, 1.0, 1.0);
    blue_sphere_mat->glossiness = 16.0;

    auto blue_sphere = make_shared<sphere>(point3(0.0, -10000.5, 0.0), 10000.0, blue_sphere_mat);
    world2.add(blue_sphere);

    /** Camera 2 */
    camera cam2;
    cam2.aspect_ratio = 16.0 / 9.0;
    cam2.image_width = 400;
    cam2.look_at = point3(0.0, 0.0, 0.0);
    cam2.look_from = point3(0.0, 0.0, 1.0);
    cam2.look_up = point3(0.0, 1.0, 0.0);
    cam2.vfov = 90;

    cam2.render(world2, "im2.ppm");

    /** Image 3 */
    hittable_list world3;
    world3.set_light_direction(vec3(1.0, 1.0, 1.0));
    world3.set_light_color(color(1.0, 1.0, 1.0));
    world3.set_ambient_light(color(0.1, 0.1, 0.1));
    world3.set_background_color(color(0.5, 0.7, 1.0)); // Light blue background

    /** Add 10 spheres */
    create_sphere(world3, point3(-0.5, -0.3, -0.5), 0.2, color(1.0, 0.0, 0.0), color(1.0, 1.0, 1.0), 32.0, 0.6, 0.3, 0.1); // Red Sphere
    create_sphere(world3, point3(0.3, -0.2, -0.3), 0.15, color(0.0, 1.0, 0.0), color(0.5, 1.0, 0.5), 64.0, 0.7, 0.2, 0.1); // Green Sphere
    create_sphere(world3, point3(-0.2, 0.2, -0.4), 0.25, color(0.0, 0.0, 1.0), color(1.0, 1.0, 1.0), 16.0, 0.9, 0.0, 0.1); // Blue Sphere
    create_sphere(world3, point3(0.6, 0.1, -0.6), 0.1, color(1.0, 1.0, 0.0), color(1.0, 1.0, 1.0), 8.0, 0.8, 0.1, 0.2); // Yellow Sphere
    create_sphere(world3, point3(-0.7, 0.3, -0.2), 0.18, color(1.0, 0.5, 0.0), color(1.0, 1.0, 1.0), 40.0, 0.5, 0.4, 0.1); // Orange Sphere
    create_sphere(world3, point3(0.4, -0.4, -0.1), 0.22, color(1.0, 0.0, 1.0), color(1.0, 1.0, 1.0), 25.0, 0.6, 0.3, 0.1); // Magenta Sphere
    create_sphere(world3, point3(-0.3, -0.1, -0.7), 0.12, color(0.0, 1.0, 1.0), color(1.0, 1.0, 1.0), 20.0, 0.7, 0.2, 0.1); // Cyan Sphere
    create_sphere(world3, point3(0.1, 0.5, -0.5), 0.3, color(0.5, 0.5, 0.5), color(1.0, 1.0, 1.0), 10.0, 0.8, 0.1, 0.3); // Gray Sphere
    create_sphere(world3, point3(-0.8, -0.5, -0.8), 0.28, color(0.9, 0.2, 0.5), color(1.0, 1.0, 1.0), 50.0, 0.6, 0.3, 0.1); // Pink Sphere
    create_sphere(world3, point3(0.7, -0.1, -0.9), 0.2, color(0.2, 0.2, 0.2), color(1.0, 1.0, 1.0), 5.0, 0.9, 0.05, 0.05); // Dark Gray Sphere

    /** Camera 3 */
    camera cam3;
    cam3.aspect_ratio = 16.0 / 9.0;
    cam3.image_width = 400;
    cam3.look_at = point3(0.0, 0.0, 0.0);
    cam3.look_from = point3(0.0, 0.0, 1.5);
    cam3.look_up = point3(0.0, 1.0, 0.0);
    cam3.vfov = 90;

    cam3.render(world3, "im3.ppm");

    /** Part 2 */

    /** Image 4 */
    hittable_list world4;
    world4.set_light_direction(vec3(0.0, 1.0, 0.0));
    world4.set_light_color(color(1.0, 1.0, 1.0));
    world4.set_ambient_light(color(0.0, 0.0, 0.0));
    world4.set_background_color(color(0.2, 0.2, 0.2));

    /** Reflective Gray Sphere */
    create_sphere(world4, point3(0.0, 0.3, -1.0), 0.25, 
                  color(0.75, 0.75, 0.75), color(1.0, 1.0, 1.0), 
                  10.0, 0.0, 0.1, 0.1, 0.9);

    /** Blue Triangle */
    create_triangle(world4, point3(0.0, -0.7, -0.5), 
                            point3(1.0, 0.4, -1.0), 
                            point3(0.0, -0.7, -1.5), 
                    color(0.0, 0.0, 1.0), color(1.0, 1.0, 1.0), 
                    4.0, 0.9, 1.0, 0.1);
    /** Yellow Triangle */
    create_triangle(world4, point3(0.0, -0.7, -0.5), 
                            point3(0.0, -0.7, -1.5),
                            point3(-1.0, 0.4, -1.0), 
                    color(1.0, 1.0, 0.0), color(1.0, 1.0, 1.0),
                    4.0, 0.9, 1.0, 0.1);


    /** Camera 4 */
    camera cam4;
    cam4.aspect_ratio = 1.0;
    cam4.image_width = 400;
    cam4.look_at = point3(0.0, 0.0, 0.0);
    cam4.look_from = point3(0.0, 0.0, 1.0);
    cam4.look_up = point3(0.0, 1.0, 0.0);
    cam4.vfov = 90;

    cam4.render(world4, "im4.ppm");

    /** Image 5 */
    hittable_list world5;
    world5.set_light_direction(vec3(1.0, 0.0, 0.0));
    world5.set_light_color(color(1.0, 1.0, 1.0));
    world5.set_ambient_light(color(0.1, 0.1, 0.1));
    world5.set_background_color(color(0.2, 0.2, 0.2));

    /** White Sphere */
    create_sphere(world5, point3(0.5, 0.0, -0.15), 0.05, 
                  color(1.0, 1.0, 1.0), color(1.0, 1.0, 1.0), 
                  4.0, 0.8, 0.1, 0.3, 0.0);

    /** Red Sphere */
    create_sphere(world5, point3(0.3, 0.0, -0.1), 0.08, 
                  color(1.0, 0.0, 0.0), color(0.5, 1.0, 0.5), 
                  32.0, 0.8, 0.8, 0.1, 0.0);

    /** Green Sphere */
    create_sphere(world5, point3(-0.6, 0.0, 0.0), 0.3, 
                  color(0.0, 1.0, 0.0), color(0.5, 1.0, 0.5), 
                  64.0, 0.7, 0.5, 0.1, 0.0);

    /** Reflective Sphere */
    create_sphere(world5, point3(0.1, -0.55, 0.25), 0.3, 
                  color(0.75, 0.75, 0.75), color(1.0, 1.0, 1.0), 
                  10.0, 0.0, 0.1, 0.1, 0.9);

    /** Blue Triangle */
    create_triangle(world5, point3(0.3, -0.3, -0.4), 
                            point3(0.0, 0.3, -0.1), 
                            point3(-0.3, -0.3, 0.2), 
                    color(0.0, 0.0, 1.0), color(1.0, 1.0, 1.0), 
                    32.0, 0.9, 0.9, 0.1, 0.0);

    /** Yellow Triangle */
    create_triangle(world5, point3(-0.2, 0.1, 0.1), 
                            point3(-0.2, -0.5, 0.2), 
                            point3(-0.2, 0.1, -0.3), 
                    color(1.0, 1.0, 0.0), color(1.0, 1.0, 1.0), 
                    4.0, 0.9, 0.5, 0.1, 0.0);

    /** Camera 5 */
    camera cam5;
    cam5.aspect_ratio = 1.0;
    cam5.image_width = 600;
    cam5.look_at = point3(0.0, 0.0, 0.0);
    cam5.look_from = point3(0.0, 0.0, 1.0);
    cam5.look_up = point3(0.0, 1.0, 0.0);
    cam5.vfov = 90;

    cam5.render(world5, "im5.ppm");


    /** Image 6 */
    hittable_list world6;
    world6.set_light_direction(vec3(1.0, -1.0, -0.5));
    world6.set_light_color(color(1.0, 1.0, 1.0));
    world6.set_ambient_light(color(0.1, 0.1, 0.1));
    world6.set_background_color(color(0.53, 0.81, 0.92)); // Sky blue

    /** Spheres */
    create_sphere(world6, point3(0.5, -0.2, -0.2), 0.1, 
                color(1.0, 0.2, 0.2), color(1.0, 1.0, 1.0),
                32.0, 0.7, 0.8, 0.1, 0.2);
                
    create_sphere(world6, point3(-0.4, 0.3, -0.5), 0.15, 
                color(0.2, 1.0, 0.2), color(1.0, 1.0, 1.0),
                16.0, 0.6, 0.7, 0.1, 0.1);
                
    create_sphere(world6, point3(-0.1, -0.4, -0.3), 0.08, 
                color(0.2, 0.2, 1.0), color(1.0, 1.0, 1.0),
                64.0, 0.8, 0.9, 0.1, 0.3);
                
    create_sphere(world6, point3(0.6, 0.5, -0.8), 0.2, 
                color(1.0, 1.0, 0.0), color(1.0, 1.0, 1.0),
                20.0, 0.7, 0.8, 0.1, 0.4);
                
    create_sphere(world6, point3(-0.7, -0.3, 0.1), 0.12, 
                color(1.0, 0.5, 0.0), color(1.0, 1.0, 1.0),
                10.0, 0.6, 0.6, 0.1, 0.2);
                
    create_sphere(world6, point3(0.0, 0.6, -0.2), 0.1, 
                color(0.5, 0.0, 0.5), color(1.0, 1.0, 1.0),
                40.0, 0.5, 0.5, 0.1, 0.1);
                
    create_sphere(world6, point3(-0.3, -0.2, 0.3), 0.14, 
                color(0.2, 1.0, 1.0), color(1.0, 1.0, 1.0),
                50.0, 0.9, 0.9, 0.1, 0.5);
                
    create_sphere(world6, point3(0.3, -0.6, 0.4), 0.18, 
                color(1.0, 1.0, 1.0), color(1.0, 1.0, 1.0),
                25.0, 0.8, 0.9, 0.1, 0.6);
                
    /** Triangles */
    create_triangle(world6, point3(0.3, -0.3, -0.4), 
                            point3(0.0, 0.3, -0.1), 
                            point3(-0.3, -0.3, 0.2),
                    color(1.0, 0.5, 0.0), color(1.0, 1.0, 1.0),
                    32.0, 0.8, 0.8, 0.1, 0.1);
                    
    create_triangle(world6, point3(-0.5, 0.2, -0.3), 
                            point3(0.1, -0.2, -0.2), 
                            point3(0.4, 0.2, -0.1),
                    color(0.5, 1.0, 0.5), color(1.0, 1.0, 1.0),
                    20.0, 0.7, 0.7, 0.1, 0.2);
                    
    create_triangle(world6, point3(-0.2, -0.5, 0.2), 
                            point3(0.2, 0.3, -0.3), 
                            point3(-0.1, 0.2, -0.2),
                    color(0.0, 0.5, 1.0), color(1.0, 1.0, 1.0),
                    16.0, 0.6, 0.6, 0.1, 0.3);
                    
    create_triangle(world6, point3(0.4, -0.4, 0.1), 
                            point3(-0.1, 0.1, -0.5), 
                            point3(-0.5, -0.4, -0.1),
                    color(1.0, 0.0, 0.5), color(1.0, 1.0, 1.0),
                    24.0, 0.9, 0.9, 0.1, 0.4);
                    
    create_triangle(world6, point3(0.2, 0.5, 0.0), 
                            point3(-0.2, 0.2, 0.4), 
                            point3(0.3, -0.1, -0.4),
                    color(0.2, 1.0, 0.2), color(1.0, 1.0, 1.0),
                    28.0, 0.8, 0.8, 0.1, 0.2);
                    
    /** Camera */
    camera custom_cam;
    custom_cam.aspect_ratio = 1.0;
    custom_cam.image_width = 800;
    custom_cam.look_at = point3(0.0, 0.0, 0.0);
    custom_cam.look_from = point3(0.0, 0.0, 1.5);
    custom_cam.look_up = point3(0.0, 1.0, 0.0);
    custom_cam.vfov = 75;

    custom_cam.render(world6, "im6.ppm");
}