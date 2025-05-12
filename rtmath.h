#ifndef RTMATH_H
#define RTMATH_H

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>

// C++ std usings
using std::make_shared;
using std::shared_ptr;

// Constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385; //Why not just use a default pi?

// Utility functions
inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

// Common Headers
#include "color.h"
#include "ray.h"
#include "vec3.h"
#include "interval.h"
#include "material.h"
#endif