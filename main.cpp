#include "rtmath.h"
#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "triangle.h"

#include <fstream>
#include <iostream>
#include <json.hpp>

using json = nlohmann::json;

color parse_color(const json& j) {
    return color(j[0], j[1], j[2]);
}

point3 parse_point(const json& j) {
    return point3(j[0], j[1], j[2]);
}

vec3 parse_vec3(const json& j) {
    return vec3(j[0], j[1], j[2]);
}

std::shared_ptr<material> parse_material(const json& mat) {
    auto m = std::make_shared<material>();
    m->diffuse_color = parse_color(mat["diffuse"]);
    m->specular_highlight_color = parse_color(mat["specular"]);
    m->glossiness = mat["glossiness"];
    m->diffuse_ref_coef = mat["diffuse_coef"];
    m->specular_ref_coef = mat["specular_coef"];
    m->ambient_ref_coef = mat["ambient_coef"];
    m->reflection_factor = mat.value("reflection_factor", 0.0); // optional
    return m;
}

int main() {
    std::ifstream scene_file("scene.json");
    if (!scene_file) {
        std::cerr << "Failed to open scene.json" << std::endl;
        return 1;
    }

    json scene;
    scene_file >> scene;

    // Create world
    hittable_list world;

    // Set lighting
    world.set_light_direction(parse_vec3(scene["lights"]["direction"]));
    world.set_light_color(parse_color(scene["lights"]["color"]));
    world.set_ambient_light(parse_color(scene["lights"]["ambient"]));

    // Background
    world.set_background_color(parse_color(scene["background"]));

    // Add spheres
    for (const auto& s : scene["spheres"]) {
        point3 center = parse_point(s["center"]);
        double radius = s["radius"];
        auto mat = parse_material(s["material"]);
        world.add(std::make_shared<sphere>(center, radius, mat));
    }

    // Add triangles
    for (const auto& t : scene["triangles"]) {
        point3 a = parse_point(t["a"]);
        point3 b = parse_point(t["b"]);
        point3 c = parse_point(t["c"]);
        auto mat = parse_material(t["material"]);
        world.add(std::make_shared<triangle>(a, b, c, mat));
    }

    // Camera
    camera cam;
    const auto& cam_json = scene["camera"];
    cam.look_from = parse_point(cam_json["look_from"]);
    cam.look_at = parse_point(cam_json["look_at"]);
    cam.look_up = parse_vec3(cam_json["look_up"]);
    cam.aspect_ratio = cam_json["aspect_ratio"];
    cam.image_width = cam_json["image_width"];
    cam.vfov = cam_json["vfov"];

    // Render
    cam.render(world, "static/im.ppm");

    return 0;
}
