#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "rtmath.h"
#include "hittable.h"
#include "material.h"

class triangle : public hittable {
    public: 
        triangle(
            const point3& a, const point3& b, const point3& c, 
            const shared_ptr<material>& mat)
            : a(a), b(b), c(c), mat(mat) {}

        bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
            //Find edges
            auto edge_1 = b - a;
            auto edge_2 = c - a;

            //Compute normal
            auto normal = cross(edge_1, edge_2);

            //Compute determinant, check ray is parallel
            const double epsilon = 1e-8;
            auto P = cross(r.direction(), edge_2);
            auto det = dot(edge_1, P);
            if ( fabs(det) < epsilon ) return false; // Ray is parallel to triangle

            //Compute u
            auto T = r.origin() - a;
            auto u = dot(T, P) / det;
            if ( u < 0 || u > 1 ) return false; // Ray intersects plane outside of triangle

            //Compute v
            auto Q = cross(T, edge_1);
            auto v = dot(r.direction(), Q) / det;
            if ( v < 0 || u + v > 1) return false; // Ray intersects plane outside of triangle

            //Compute t
            auto t = dot(edge_2, Q) / det;
            if (!ray_t.surrounds(t)) return false; //Ray intersection outside of range

            //Ensure normal is facing correct direction and is normalized
            if (det < 0) normal = -normal;
            normal = unit_vector(normal);

            //Store hit record
            rec.p = r.at(t);
            rec.set_face_normal(r, normal);
            rec.t = t;
            rec.mat = mat;

            return true;
        }

        shared_ptr<material> get_material() const { return mat; }

    private:
        point3 a, b, c;
        shared_ptr<material> mat;

};

#endif