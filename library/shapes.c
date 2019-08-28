#include <math.h>
#include "shapes.h"

Body *n_polygon_shape(size_t num_sides, double radius, double mass,
    RGBColor color, Vector centroid) {
    List *vertices = list_init(num_sides, (FreeFunc) vec_free);
    double theta = 2 * M_PI / num_sides;
    Vector start = (Vector) {.x = radius, .y = 0};
    size_t i;

    for (i = 0; i < num_sides; i++) {
        Vector tmp = vec_rotate(start, theta * i);
        list_add(vertices, vec_init(tmp.x, tmp.y));
    }

    Body *res = body_init(vertices, mass, color);
    body_set_centroid(res, centroid);
    return res;
}
