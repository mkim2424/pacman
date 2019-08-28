#include <assert.h>
#include <math.h>
#include "list.h"
#include "vector.h"
#include "test_util.h"

void test_list_size0() {
    List *l = list_init(0, (FreeFunc) vec_free);
    assert(list_size(l) == 0);
    list_free(l);
}

void test_list_size1() {
    List *l = list_init(1, (FreeFunc) vec_free);
    assert(list_size(l) == 0);
    // Add
    Vector *vec1 = vec_init(0, 0);
    list_add(l, vec1);
    assert(list_size(l) == 1);
    // Remove
    assert(list_remove(l, 0) == vec1);
    assert(list_size(l) == 0);
    // Add again
    Vector *vec2 = vec_init(1, 1);
    list_add(l, vec2);
    assert(list_size(l) == 1);
    assert(list_get(l, 0) == vec2);
    // Set
    Vector *vec3 = vec_init(1, 2);
    list_remove(l, 0);
    list_add(l, vec3);
    assert(list_size(l) == 1);
    assert(list_get(l, 0) == vec3);
    // Free stuff
    vec_free(vec1);
    list_free(l);
}

void test_list_small() {
    List *l = list_init(5, (FreeFunc) vec_free);
    assert(list_size(l) == 0);
    // Fill partially
    Vector *vec1 = vec_init(0, 0);
    Vector *vec2 = vec_init(1, 1);
    Vector *vec3 = vec_init(2, 2);
    list_add(l, vec1);
    list_add(l, vec2);
    list_add(l, vec3);
    assert(list_size(l) == 3);
    assert(list_get(l, 0) == vec1);
    assert(list_get(l, 1) == vec2);
    assert(list_get(l, 2) == vec3);
    // Fill to capacity
    Vector *vec4 = vec_init(3, 3);
    Vector *vec5 = vec_init(4, 4);
    list_add(l, vec4);
    list_add(l, vec5);
    assert(list_size(l) == 5);
    assert(list_get(l, 3) == vec4);
    assert(list_get(l, 4) == vec5);
    // Remove some
    assert(list_remove(l, 4) == vec5);
    assert(list_remove(l, 3) == vec4);
    assert(list_size(l) == 3);
    // Add, replacing previous elements
    list_add(l, vec4);
    list_add(l, vec5);
    assert(list_size(l) == 5);
    assert(list_get(l, 0) == vec1);
    assert(list_get(l, 1) == vec2);
    assert(list_get(l, 2) == vec3);
    assert(list_get(l, 3) == vec4);
    assert(list_get(l, 4) == vec5);
    // Overwrite added elements
    Vector *vec6 = vec_init(5, 5);
    Vector *vec7 = vec_init(6, 6);
    list_remove(l, 4);
    list_remove(l, 3);
    list_add(l, vec6);
    list_add(l, vec7);
    assert(list_get(l, 0) == vec1);
    assert(list_get(l, 1) == vec2);
    assert(list_get(l, 2) == vec3);
    assert(list_get(l, 3) == vec6);
    assert(list_get(l, 4) == vec7);
    // Free stuff
    list_free(l);
}

void test_list_resize() {
    // Create small list and add elements so it needs to resize
    List *l = list_init(1, (FreeFunc) vec_free);
    assert(list_size(l) == 0);
    // Fill partially
    Vector *vec1 = vec_init(0, 0);
    Vector *vec2 = vec_init(1, 1);
    Vector *vec3 = vec_init(2, 2);
    list_add(l, vec1);
    list_add(l, vec2);
    list_add(l, vec3);
    assert(list_size(l) == 3);
    assert(list_get(l, 0) == vec1);
    assert(list_get(l, 1) == vec2);
    assert(list_get(l, 2) == vec3);
    assert(list_remove(l, 0) == vec1);
    assert(list_remove(l, 0) == vec2);
    assert(list_remove(l, 0) == vec3);
    // Free stuff
    vec_free(vec1);
    vec_free(vec2);
    vec_free(vec3);
    list_free(l);
}

int main(int argc, char *argv[]) {
    // Run all tests if there are no command-line arguments
    bool all_tests = argc == 1;
    // Read test name from file
    char testname[100];
    if (!all_tests) {
        read_testname(argv[1], testname, sizeof(testname));
    }

    DO_TEST(test_list_size0)
    DO_TEST(test_list_size1)
    DO_TEST(test_list_small)
    DO_TEST(test_list_resize)

    puts("list PASS");
    return 0;
}
