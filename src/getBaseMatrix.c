// FUNC 80021f5c 80 MAIN0
// MATCHING 80021f5c 80
// VERSION SLPS_01144 slps01144 800228F0 80
#if defined(VERSION_SLPS_01144)
#include "include_asm.h"
INCLUDE_ASM_V("slps01144/nonmatchings/slps01144", getBaseMatrix);
#else
typedef struct S { int w[8]; } S;
extern S DAT_1f8000f8;

void getBaseMatrix(S *o)
{
    *o = DAT_1f8000f8;
}
#endif
