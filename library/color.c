#include <stdlib.h>
#include <math.h>
#include "color.h"


float get_rand() {
    return rand() / (double) RAND_MAX;
}

RGBColor random_color() {
    return (RGBColor) {.r = get_rand(), .g = get_rand(), .b = get_rand()};
}
