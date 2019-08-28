#include "forces.h"
#include "test_util.h"
#include <assert.h>
#include <math.h>
#include <stdlib.h>

List *make_shape() {
    List *shape = list_init(4, free);
    Vector *v = malloc(sizeof(*v));
    *v = (Vector) {-1, -1};
    list_add(shape, v);
    v = malloc(sizeof(*v));
    *v = (Vector) {+1, -1};
    list_add(shape, v);
    v = malloc(sizeof(*v));
    *v = (Vector) {+1, +1};
    list_add(shape, v);
    v = malloc(sizeof(*v));
    *v = (Vector) {-1, +1};
    list_add(shape, v);
    return shape;
}

double gravity_potential(double G, Body *body1, Body *body2) {
    Vector r = vec_subtract(body_get_centroid(body2), body_get_centroid(body1));
    return -G * body_get_mass(body1) * body_get_mass(body2) / sqrt(vec_dot(r, r));
}
double kinetic_energy(Body *body) {
    Vector v = body_get_velocity(body);
    return body_get_mass(body) * vec_dot(v, v) / 2;
}

double spring_potential(double K, Body *body1, Body *body2) {
    Vector r = vec_subtract(body_get_centroid(body2), body_get_centroid(body1));
    return 0.5 * K * vec_dot(r, r);
}

// Tests that two same mass bodies have same kinetic energy
void test_same_kinetic() {
    const double M1 = 10, M2 = 10;
    const double G = 1e3;
    const double DT = 1e-6;
    const int STEPS = 1000000;
    Scene *scene = scene_init();
    Body *mass1 = body_init(make_shape(), M1, (RGBColor) {0, 0, 0});
    body_set_centroid(mass1, (Vector) {10, 100});
    scene_add_body(scene, mass1);
    Body *mass2 = body_init(make_shape(), M2, (RGBColor) {0, 0, 0});
    body_set_centroid(mass2, (Vector) {1000, 100});
    scene_add_body(scene, mass2);
    create_newtonian_gravity(scene, G, mass1, mass2);
    for (int i = 0; i < STEPS; i++) {
        assert(within(1e-5, kinetic_energy(mass1), kinetic_energy(mass2)));
        scene_tick(scene, DT);
    }
    scene_free(scene);
}

// Tests if center body stays in same position for symmetric scene
void test_same_position() {
    const double M1 = 10, M2 = 1, M3 = 10;
    const double G = 1e3;
    const double DT = 1e-6;
    const int STEPS = 1000000;
    Scene *scene = scene_init();
    Body *mass1 = body_init(make_shape(), M1, (RGBColor) {0, 0, 0});
    body_set_centroid(mass1, (Vector) {0, 100});
    scene_add_body(scene, mass1);
    Body *mass2 = body_init(make_shape(), M2, (RGBColor) {0, 0, 0});
    body_set_centroid(mass2, (Vector) {500, 100});
    scene_add_body(scene, mass2);
    Body *mass3 = body_init(make_shape(), M3, (RGBColor) {0, 0, 0});
    body_set_centroid(mass3, (Vector) {1000, 100});
    scene_add_body(scene, mass3);
    create_newtonian_gravity(scene, G, mass1, mass2);
    create_newtonian_gravity(scene, G, mass2, mass3);
    create_newtonian_gravity(scene, G, mass1, mass3);
    for (int i = 0; i < STEPS; i++) {
        assert(vec_isclose(
            body_get_centroid(mass2),
            (Vector) {500, 100}
        ));
        scene_tick(scene, DT);
    }
    scene_free(scene);
}

// Tests conservation of spring and kinetic energy
void test_spring_energy() {
    const double M = 10;
    const double K = 2;
    const double A = 3;
    const double DT = 1e-6;
    const int STEPS = 1000000;
    Scene *scene = scene_init();
    Body *mass = body_init(make_shape(), M, (RGBColor) {0, 0, 0});
    body_set_centroid(mass, (Vector) {A, 0});
    scene_add_body(scene, mass);
    Body *anchor = body_init(make_shape(), INFINITY, (RGBColor) {0, 0, 0});
    scene_add_body(scene, anchor);
    create_spring(scene, K, mass, anchor);
    double initial_energy = spring_potential(K, mass, anchor);
    for (int i = 0; i < STEPS; i++) {
        assert(within(1e-5, initial_energy, spring_potential(K, mass, anchor) + kinetic_energy(mass)));
        scene_tick(scene, DT);
    }
    scene_free(scene);
}

int main(int argc, char *argv[]) {
    // Run all tests if there are no command-line arguments
    bool all_tests = argc == 1;
    // Read test name from file
    char testname[100];
    if (!all_tests) {
        read_testname(argv[1], testname, sizeof(testname));
    }
    DO_TEST(test_same_kinetic)
    DO_TEST(test_same_position)
    DO_TEST(test_spring_energy)
    puts("student_tests PASS");
    return 0;
}
