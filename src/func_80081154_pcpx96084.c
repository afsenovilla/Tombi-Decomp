// VERSION PCPX_96084 tomba 80081154 204
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Spawns a type-0x16 object of the given subtype at (x, y, z) with the animation D_80013EA0.
#include "tobj.h"
extern TObj *func_8001A36C_pcpx96084(void);
extern void AnimLoadDuration(TObj *);
extern void *D_80013EA0;
extern int D_1F800170;
TObj *func_80081154_pcpx96084(unsigned char sub, int x, int y, int z)
{
    TObj *n = func_8001A36C_pcpx96084();
    if (n != 0) {
        n->active = 1;
        n->type = 0x16;
        n->a.raw = x << 16;
        n->y.raw = y << 16;
        n->b.raw = z << 16;
        *(volatile short *)&n->w1e = 7;
        *(volatile int *)&n->d8c = 0;
        n->b0d = 0;
        {
            int c = D_1F800170;
            void *a = D_80013EA0;
            *(volatile short *)&n->b0a = 2;
            n->d3c = c;
            n->anim = a;
        }
        AnimLoadDuration(n);
        n->subtype = sub;
        return n;
    }
    return 0;
}

#endif
