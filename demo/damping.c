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
#define BALL_RADIUS 10

#define NUM_BALLS 50  // calculate this value with WIDTH and BALL_RADIUS
#define K 15          // spring constant
#define BALL_MASS 5
#define N_SIDES 15
#define ANCHOR_RADIUS 0.001
#define K_DECREMENT 0.03
#define GAMMA_INCREMENT 0.04 // gamma increment for first half of balls

const Vector min = {.x = 0, .y = 0};
const Vector max = {.x = WIDTH, .y = HEIGHT};

// add an achor for each ball in the scene
void add_anchors(Scene *scene) {
    size_t i;
    double x_centroid = BALL_RADIUS;
    for (i = 0; i < NUM_BALLS; i++) {
        Vector centroid = (Vector) {.x = x_centroid, .y = HEIGHT / 2};
        Body* ball = n_polygon_shape(N_SIDES, ANCHOR_RADIUS, INFINITY,
            (RGBColor) {0, 0, 0}, centroid);
        scene_add_body(scene, ball);
        x_centroid += 2 * BALL_RADIUS;
    }
}

// instantiate NUM_BALLS balls in the shape of a v
void instatitate_balls(Scene *scene) {
    size_t i;
    double d_y = (HEIGHT / 2) / (NUM_BALLS / 2); // y increment
    double x_centroid = BALL_RADIUS;
    double y_centroid = HEIGHT;
    for (i = 0; i < NUM_BALLS; i++) {
        Vector centroid = (Vector) {.x = x_centroid, .y = y_centroid};
        Body* ball = n_polygon_shape(N_SIDES, BALL_RADIUS, BALL_MASS, random_color(), centroid);
        scene_add_body(scene, ball);
        x_centroid += 2 * BALL_RADIUS;

        if ( i < NUM_BALLS / 2) {
            y_centroid -= d_y;
        }
        else {
            y_centroid += d_y;
        }
    }
    add_anchors(scene);
}


// add a spring force to each ball and corresponding anchor
void add_spring_force(Scene *scene) {
    size_t i;
    double k = K;
    for (i = 0; i < NUM_BALLS; i++) {
        Body *body1 = scene_get_body(scene, i);
        Body *body2 = scene_get_body(scene, i + NUM_BALLS);
        create_spring(scene, k, body1, body2);
        k -= K_DECREMENT;
        assert(k >= 0);
    }
}

void add_drag(Scene *scene) {
    size_t i;
    double gamma = 0;
    for (i = 0; i < NUM_BALLS; i++) {
        // increment gamma for first half of balls
        if (i < NUM_BALLS / 2) {
            gamma += GAMMA_INCREMENT;
        }
        // large constant gamma for second half of balls
        else {
            gamma = 3;
        }
        create_drag(scene, gamma, scene_get_body(scene, i));
    }
}

int main(int argc, char *argv[]) {
    sdl_init(min, max);
    Scene *scene = scene_init();
    double dt;
    instatitate_balls(scene);
    add_spring_force(scene);
    add_drag(scene);

    while (!sdl_is_done()) {
        dt = time_since_last_tick();
        scene_tick(scene, dt);
        sdl_render_scene(scene);
    }
    scene_free(scene);
}
