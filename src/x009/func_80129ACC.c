// FUNC 80129acc 152 X009
// MATCHING 80129acc 152
#include "plx.h"
#include "gvars.h"
#include "player.h"

static __inline__ short chk(void)
{
    if (GVARS.b3a == 0) return 0;
    if (GVARS.b38 != 0) return 0;
    if (PLAYER.obj.b69 == 0 && PLX.b6100 == 0) return 0;
    if ((unsigned short)((*(unsigned short **)&PLAYER.obj.h)[1] - 0x1034) >= 0xc8) return 0;
    if (PLAYER.obj.y.p.whole < -0x1d0) return 0;
    return 1;
}

int func_80129ACC(void)
{
    return chk();
}
