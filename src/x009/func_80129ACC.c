// FUNC 80129acc 152 X009
// MATCHING 80129acc 152
#include "player.h"
extern unsigned char D_8009C93A;
extern unsigned char D_8009C938;
extern unsigned char D_800A6100;

static __inline__ short chk(void)
{
    if (D_8009C93A == 0) return 0;
    if (D_8009C938 != 0) return 0;
    if (PLAYER.obj.b69 == 0 && D_800A6100 == 0) return 0;
    if ((unsigned short)((*(unsigned short **)&PLAYER.obj.h)[1] - 0x1034) >= 0xc8) return 0;
    if (PLAYER.obj.y.p.whole < -0x1d0) return 0;
    return 1;
}

int func_80129ACC(void)
{
    return chk();
}
