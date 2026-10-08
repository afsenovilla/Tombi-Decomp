// FUNC 80129b70 296 X009
// MATCHING 80129b70 296
#include "TOBJ.H"

extern unsigned char D_8009C93A, D_8009C938, D_800A60A1, D_800A6100;
extern Fix16 *D_800A6078;
extern short D_800A604E;
extern void func_80129914(TObj *);
extern void PoolFree_1F800210(TObj *);

static __inline__ short Cond(void)
{
    if (!D_8009C93A) return 0;
    if (D_8009C938) return 0;
    if (!D_800A60A1 && !D_800A6100) return 0;
    if ((unsigned int)((unsigned short)D_800A6078->p.whole - 0x1034) >= 0xc8) return 0;
    if (D_800A604E < -0x1d0) return 0;
    return 1;
}

void func_80129B70(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04 = 1;
        o->step = 0;
        o->state = 0;
        break;
    case 1:
        if (Cond()) func_80129914(o);
        break;
    case 2:
        o->b04 = 3;
        break;
    case 3:
        PoolFree_1F800210(o);
        break;
    }
}
