#include <assert.h>
#include <stdlib.h>
#include "scene.h"

#define INIT_SIZE 10

typedef struct {
    ForceCreator forcer;
    void *aux;
    FreeFunc freer;
} force_creator_info;

struct scene {
    List *bodies;
    List *force_creators;
};

Scene *scene_init(void) {
    Scene *res = malloc(sizeof(Scene));
    assert(res != NULL);
    res->bodies = list_init(INIT_SIZE, (FreeFunc) body_free);
    res->force_creators = list_init(INIT_SIZE, (FreeFunc) free);
    return res;
}

void scene_free(Scene *scene) {
    assert(scene != NULL);
    list_free(scene->bodies);

    for (size_t i = 0; i < list_size(scene->force_creators); i++) {
        force_creator_info *tmp = list_get(scene->force_creators, i);
        tmp->freer(tmp->aux);
    }

    list_free(scene->force_creators);
    free(scene);
}

size_t scene_bodies(Scene *scene) {
    assert(scene != NULL);
    return list_size(scene->bodies);
}

Body *scene_get_body(Scene *scene, size_t index) {
    assert(scene != NULL && index < scene_bodies(scene));
    return list_get(scene->bodies, index);
}

void scene_add_body(Scene *scene, Body *body) {
    assert(scene != NULL && body != NULL);
    list_add(scene->bodies, body);
}

void scene_remove_body(Scene *scene, size_t index) {
    assert(scene != NULL);
    body_free(list_remove(scene->bodies, index));
}

/**
 * Adds a force creator to a scene,
 * to be invoked every time scene_tick() is called.
 * The auxiliary value is passed to the force creator each time it is called.
 *
 * @param scene a pointer to a scene returned from scene_init()
 * @param forcer a force creator function
 * @param aux an auxiliary value to pass to forcer when it is called
 * @param freer if non-NULL, a function to call in order to free aux
 */
void scene_add_force_creator(Scene *scene, ForceCreator forcer,
    void *aux, FreeFunc freer) {
    force_creator_info *res = malloc(sizeof(force_creator_info));
    assert(res != NULL);
    res->forcer = forcer;
    res->aux = aux;
    res->freer = freer;
    list_add(scene->force_creators, res);
}

void scene_tick(Scene *scene, double dt) {
    assert(scene != NULL);
    size_t i;

    for (i = 0; i < list_size(scene->force_creators); i++) {
        force_creator_info *tmp = list_get(scene->force_creators, i);
        tmp->forcer(tmp->aux);
    }

    for (i = 0; i < scene_bodies(scene); i++) {
        body_tick(scene_get_body(scene, i), dt);
    }
}
