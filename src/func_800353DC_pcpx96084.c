// VERSION PCPX_96084 tomba 800353DC 340
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 form of PAL x001 80118AC0 (updateAnimScenerySprite.c): scratchpad word at 0x1F80015C (PAL 0x1F8002D4), state 1 does not set the visible flag.
#include "TOBJ.H"

extern void *D_80014154[];
extern void AnimLoadDuration(TObj *);
extern void func_80018550(TObj *);
extern void func_8001A87C_pcpx96084(TObj *);
extern int D_1F80015C;

void func_800353DC_pcpx96084(TObj *o)
{
    unsigned char h;

    switch (o->b04) {
    case 0:
        if (o->subtype < 2) {
            o->w1e = 8;
        } else {
            o->w1e = 10;
        }
        o->b0d = 0;
        o->d3c = D_1F80015C;
        o->anim = D_80014154[o->subtype];
        h = (*(unsigned char*)&o->box0) >> 4;
        o->b04 = o->b04 + 1;
        if (h < 12) {
            o->d64 = (h << 10) + 0x1000;
        } else {
            o->d64 = 0x1000 - ((h - 11) << 9);
        }
        h = (*(unsigned char*)&o->box0) & 0xf;
        if (h < 8) {
            o->d8c = h << 2;
        } else {
            o->d8c = (-((int)(h - 7) << 5) / 8) & 0xff;
        }
        AnimLoadDuration(o);
        break;
    case 1:
        func_80018550(o);
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        func_8001A87C_pcpx96084(o);
        break;
    }
}
#endif
