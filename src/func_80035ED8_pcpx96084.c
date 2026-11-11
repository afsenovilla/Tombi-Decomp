// VERSION PCPX_96084 tomba 80035ED8 280
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Spawns a type-0x1B object of subtype sub at (x, y, z), animation D_80013F2C[sub]; subtype 0 is remembered in
// D_800F5090.
#include "TOBJ.H"
extern TObj *func_8001A464_pcpx96084(void);
extern void AnimLoadDuration(TObj *);
extern void *D_80013F2C[];
extern unsigned char D_800F4FE7;
extern int D_1F80015C;
extern TObj *D_800F5090;

void func_80035ED8_pcpx96084(short sub, short x, short y, short z)
{
    TObj *n = func_8001A464_pcpx96084();
    if (n != 0) {
        n->active = 1;
        n->type = 0x1b;
        *(int *)((char *)n + 0x10) = x << 16;
        *(int *)((char *)n + 0x14) = y << 16;
        *(int *)((char *)n + 0x18) = z << 16;
        n->subtype = sub;
        if (sub >= 2) n->w1e = 8;
        else n->w1e = 7;
        n->b0f = D_800F4FE7 - 1;
        n->d3c = D_1F80015C;
        n->anim = D_80013F2C[sub];
        AnimLoadDuration(n);
        n->b0d = 0;
        *(short *)&n->b0a = 0;
        n->b68 = 0;
        n->category |= 0x80;
        if (sub == 0) D_800F5090 = n;
    }
}
#endif
