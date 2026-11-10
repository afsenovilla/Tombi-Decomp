// VERSION PCPX_96084 tomba 80079D50 256
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 form of spawnChick (src/spawnChick.c): a chick at (x, y, z) facing at random, drawn just in front of
// the player.
#include "TOBJ.H"
extern TObj *func_8001A36C_pcpx96084(void);
extern int Rand(void);
extern void AnimLoadDuration(TObj *);
extern void *D_80014050;
extern volatile unsigned char D_800F4FE7;
extern int D_1F80015C;

void func_80079D50_pcpx96084(short x, short y, short z)
{
    TObj *n = func_8001A36C_pcpx96084();
    if (n != 0) {
        n->active = 1;
        n->box0 = 6;
        n->box1 = 12;
        n->box2 = 6;
        n->box3 = 12;
        n->type = 3;
        *(volatile unsigned short *)&n->animFrame = Rand() & 1;
        *(int *)((char *)n + 0x10) = x << 16;
        *(int *)((char *)n + 0x14) = y << 16;
        *(int *)((char *)n + 0x18) = z << 16;
        *(volatile unsigned short *)&n->w1e = 11;
        n->b0c = 1;
        n->b0d = 0;
        n->b69 = 0;
        n->b0f = D_800F4FE7 - 1;
        n->d3c = D_1F80015C;
        n->anim = D_80014050;
        *(short *)&n->b0a = 2;
        AnimLoadDuration(n);
        n->b04 = 1;
        n->step = 0;
        n->state = 0;
    }
}
#endif
