#include <assert.h>
#include <stdlib.h>
#include "body.h"

struct body {
    List *shape; // original shape...never gets modified
    Vector centroid;
    Vector velocity;
    double mass;
    RGBColor color;
    double angle; // absolute angle of the shape
    Vector force;
    Vector impulse;
};

Body *body_init(List *shape, double mass, RGBColor color) {
    assert(shape != NULL && mass > 0);
    Body *res = malloc(sizeof(Body));
    assert(res != NULL);
    res->shape = shape;
    res->centroid = polygon_centroid(shape);
    res->velocity = (Vector) {0, 0};
    res->mass = mass;
    res->color = color;
    res->angle = 0;
    res->force = (Vector) {0, 0};
    res->impulse = (Vector) {0, 0};
    return res;
}

void body_free(Body *body) {
    assert(body != NULL);
    list_free(body->shape);
    free(body);
}

List *body_get_shape(Body *body) {
    assert(body != NULL);
    size_t size = list_size(body->shape);
    List *res = list_init(size, (FreeFunc) vec_free);
    assert(res != NULL);
    size_t i;

    for (i = 0; i < size; i++) {
        Vector *tmp = (Vector *) list_get(body->shape, i);
        Vector new_vec = vec_add(vec_rotate(*tmp, body->angle), body->centroid);
        list_add(res, vec_init(new_vec.x, new_vec.y));
    }

    return res;
}

Vector body_get_centroid(Body *body) {
    assert(body != NULL);
    return body->centroid;
}

Vector body_get_velocity(Body *body) {
    assert(body != NULL);
    return body->velocity;
}

double body_get_mass(Body *body) {
    assert(body != NULL);
    return body->mass;
}

RGBColor body_get_color(Body *body) {
    assert(body != NULL);
    return body->color;
}

void body_set_centroid(Body *body, Vector x) {
    assert(body != NULL);
    body->centroid = x;
}

void body_set_velocity(Body *body, Vector v) {
    assert(body != NULL);
    body->velocity = v;
}

void body_set_rotation(Body *body, double angle) {
    assert(body != NULL);
    body->angle = angle;
}

void body_add_force(Body *body, Vector force) {
    body->force = vec_add(body->force, force);
}

void body_add_impulse(Body *body, Vector impulse) {
    body->impulse = vec_add(body->impulse, impulse);
}

void body_tick(Body *body, double dt) {
    assert(body != NULL);

    // a is acceleration vector
    Vector a   = vec_multiply(1.0 / body->mass, body->force);
    // dv is change in velocity vector
    Vector dv  = vec_add(vec_multiply(dt, a), body->impulse);
    // avg is translation velocity vector
    Vector avg = vec_add(body->velocity, vec_multiply(0.5, dv));
    // dx is change in position vector
    Vector dx  = vec_multiply(dt, avg);

    body_set_centroid(body, vec_add(body->centroid, dx));
    body_set_velocity(body, vec_add(body->velocity, dv));
    body->force = (Vector) {0, 0};
    body->impulse = (Vector) {0, 0};
}
