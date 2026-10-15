// FUNC 800fc18c 180 X001
// MATCHING 800fc18c 180
#include "gcd.h"
#include "gv2.h"
int playerClampClimbTop(unsigned char *o)
{
    int r = 0;
    if (GV2.bc3 & 0x40) {
        if (*(short *)(o + 0x16) > -0x8c) {
            r = 1;
            *(short *)(o + 0x56) = -0x8c;
            if (*(short *)(o + 0x16) > -0x7c) {
                if (GCD.bce3d == 0xff) r = 2;
                else *(short *)(o + 0x16) = -0x8c;
            }
        }
    } else {
        if (*(short *)(o + 0x16) > -0x3c0) {
            r = 1;
            *(short *)(o + 0x56) = -0x3c0;
            if (*(short *)(o + 0x16) > -0x3b0) {
                if (GCD.bce3d == 0xff) r = 2;
                else *(short *)(o + 0x16) = -0x3c0;
            }
        }
    }
    return r;
}
