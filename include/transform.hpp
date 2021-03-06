#ifndef BOX_TRANSFORM_HPP
#define BOX_TRANSFORM_HPP

#include "matrix.hpp"
#include "vector.hpp"

namespace box {
    matrix4 make_viewport(double x, double y, double w, double h);
    matrix4 make_lookat(vector3 eye, vector3 center, vector3 up);
    matrix4 make_persp(double fov, double aspect, double near, double far);
    matrix4 make_ortho(double left, double right, double bottom, double top, double near, double far);
    matrix4 make_rotate(double angle, vector3 v);
    matrix4 make_translate(vector3 offset);
}

#endif