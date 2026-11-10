// VERSION PCPX_96084 tomba 8007F294 208
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Spawns a type-0x12 object at (x, y, z) facing like the player, animation D_80013F50, started in state 2/3.
#include "TOBJ.H"
extern TObj *func_8001A36C_pcpx96084(void);
extern void AnimLoadDuration(TObj *);
extern void * volatile D_80013F50;
extern unsigned short D_800F5006;
extern volatile int D_1F80015C;

void func_8007F294_pcpx96084(short x, short y, short z)
{
    unsigned short fr;
    TObj *n = func_8001A36C_pcpx96084();
    if (n != 0) {
        n->active = 1;
        n->type = 0x12;
        fr = D_800F5006;
        *(int *)((char *)n + 0x10) = x << 16;
        *(int *)((char *)n + 0x14) = y << 16;
        *(int *)((char *)n + 0x18) = z << 16;
        *(volatile unsigned short *)&n->w1e = 0xb;
        *(volatile unsigned char *)&n->b0d = 0;
        *(volatile unsigned short *)&n->animFrame = fr & 1;
        *(volatile int *)&n->d3c = D_1F80015C;
        n->anim = D_80013F50;
        *(volatile short *)&n->b0a = 2;
        AnimLoadDuration(n);
        n->b04 = 2;
        n->step = 3;
        n->state = 0;
    }
}
#endif
