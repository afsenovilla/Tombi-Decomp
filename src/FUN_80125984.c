// FUNC 80125984 164 X000
// MATCHING 80125984 164
#include "raw7.h"
extern short FUN_8004461c(char *, char *);
extern void FUN_800428c0(char *, char *);
extern short DAT_1f80019e;
void FUN_80125984(char *a, char *b)
{
    if (S32(b, 0x94) == 0 && FUN_8004461c(a, b) >= 0) {
        unsigned short v;
        FUN_800428c0(a, b);
        if (U8(a, 2) != 10) {
            if (U8(a, 2) != 1) {
                unsigned short x = U16(a, 0x2e);
                v = x < 4 ? x & 1 : 3;
            } else {
                v = U16(a, 0x2e) & 1;
            }
            U16(b, 0x2e) = v;
            DAT_1f80019e = 0;
        }
    }
}
