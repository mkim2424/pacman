#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "forces.h"
#include "scene.h"
#include "sdl_wrapper.h"
#include "shapes.h"
#include "color.h"

#define WIDTH 1000
#define HEIGHT 500
#define NUM_BODIES 100
#define NUM_SIDES 10
#define RADIUS 10
#define MASS 20000000
#define GRAVITY 1e-3

const Vector min = {.x = 0, .y = 0};
const Vector max = {.x = WIDTH, .y = HEIGHT};
const RGBColor red = {.r = 1, .g = 0, .b = 0};


// Return random location as a vector on the screen
Vector random_location() {
    double x = rand() % (WIDTH + 1);
    double y = rand() % (HEIGHT + 1);
    return (Vector) {.x = x, .y = y};
}

int main(int argc, char *argv[]) {
    sdl_init(min, max);
    Scene *scene = scene_init();
    size_t i, j;
    double dt;

    for (i = 0; i < NUM_BODIES; i++) {
        scene_add_body(scene, n_polygon_shape(NUM_SIDES, RADIUS, MASS, random_color(), random_location()));
    }

    for (i = 0; i < NUM_BODIES -1; i++) {
        for (j = i + 1; j < NUM_BODIES; j++) {
            create_newtonian_gravity(scene, GRAVITY, scene_get_body(scene, i), scene_get_body(scene, j));
        }
    }

    while (!sdl_is_done()) {
        dt = time_since_last_tick();
        scene_tick(scene, dt);
        sdl_render_scene(scene);
    }

    return 0;
}
