// FUNC 8011e364 80 X006
// MATCHING 8011e364 80
#include "gcd.h"

int compareScfe2Range(void)
{
    short x = GCD.scfe2;
    int r;

    if (x > GCD.scfd4) {
        r = -1;
    } else if (GCD.scfd2 < x) {
        r = 2;
    } else {
        r = GCD.scfd0 < x;
    }
    return r;
}
