// FUNC 80020eac 104 MAIN0
// MATCHING 80020eac 104
// VERSION SLPS_01144 slps01144 80021DF4 104
#if defined(VERSION_SLPS_01144)
#include "include_asm.h"
INCLUDE_ASM_V("slps01144/nonmatchings/slps01144", loadCollisionBounds);
#else
typedef struct { unsigned short a, b, c, d; } E;
extern short IDX;
extern E T[];
extern unsigned short S3C4, S3C6, S3C8, S3CA;

void loadCollisionBounds(void)
{
    int i = IDX;
    S3C4 = T[i].a;
    S3C6 = T[i].b;
    S3C8 = T[i].c;
    S3CA = T[i].d;
}
#endif
