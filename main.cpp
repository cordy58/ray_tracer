#include "rtmath.h"
#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"

int main() {
    hittable_list world;
    world.set_light_direction(vec3(0.0, 1.0, 0.0));
    world.set_light_color(color(1.0, 1.0, 1.0));
    world.set_ambient_light(color(0.1, 0.1, 0.1));
    world.set_background_color(color(0.2, 0.2, 0.2));

    auto mat = make_shared<material>();
    mat->diffuse_ref_coef = 0.7;
    mat->diffuse_color = color(1.0, 0.0, 0.0);  // red
    mat->specular_ref_coef = 0.2;
    mat->ambient_ref_coef = 0.1;
    mat->glossiness = 16.0;

    world.add(make_shared<sphere>(point3(0, 0, 0), 0.4, mat));

    camera cam;
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.look_at = point3(0, 0, 0);
    cam.look_from = point3(0, 0, 1);
    cam.look_up = vec3(0, 1, 0);
    cam.vfov = 90;

    cam.render(world, "static/im.ppm");  // save to static folder

    return 0;
}
