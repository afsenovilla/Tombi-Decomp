// FUNC 8012b63c 48 X003
// MATCHING 8012b63c 48
#include "gcd.h"

int isPreciousChestEventActive(void)
{
    if (GCD.bd0c8 != 0 && GCD.bce5d != 0xff) return 1;
    return 0;
}
