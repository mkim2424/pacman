#include "color.h"
#include "test_util.h"
#include <assert.h>
#include <math.h>

void test_in_range() {
    RGBColor color = random_color();
    assert(color.r >=0 && color.r<=1);
    assert(color.b >=0 && color.b<=1);
    assert(color.g >=0 && color.g<=1);
}

int main(int argc, char *argv[]) {

    // Run all tests if there are no command-line arguments
    bool all_tests = argc == 1;
    // Read test name from file
    char testname[100];
    if (!all_tests) {
        read_testname(argv[1], testname, sizeof(testname));
    }

    DO_TEST(test_in_range)

    puts("color_test PASS");

    return 0;
}
