// FUNC 8012747c 52 X004
// MATCHING 8012747c 52
#include "gcd.h"
int testGcdFlagBitBcefc(int n)
{
    int q = n / 8; int r = n - q * 8; return ((unsigned char*)&GCD.bcefc)[q] & (1 << r);
}
