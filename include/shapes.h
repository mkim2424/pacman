#ifndef __SHAPES_H__
#define __SHAPES_H__

#include "color.h"
#include "list.h"
#include "body.h"

Body *n_polygon_shape(size_t num_sides, double radius, double mass,
    RGBColor color, Vector centroid);

Body *star_shape(size_t num_sides);

Body *pac_man_shape();

#endif // #ifndef __SHAPES_H__
