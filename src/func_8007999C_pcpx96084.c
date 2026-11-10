// VERSION PCPX_96084 tomba 8007999C 380
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 version of PAL chickDropToGround (src/chickDropToGround.c): field +0xA8 (PAL +0xAC), no +0x7A check on landing.
#include "TOBJ.H"
extern void func_800180D8_pcpx96084(TObj *o);
extern void AnimLoadDuration(TObj *o);
extern unsigned Rand(void);
extern short func_8001B9AC(TObj *o, int x, int y);
extern void *D_80014050;
extern char D_800A1898[];

void chickDropToGround(TObj *o)
{
    short v;
    unsigned short u;
    switch (o->state) {
    case 0:
        o->active = 2;
        o->movetab = D_800A1898;
        o->animFrame = Rand() & 1;
        (o->wa8) = 0;
        o->anim = D_80014050;
        if (o->b0c == 0) o->velY = -0x400;
        else o->velY = 0x80;
        v = o->h->p.whole;
        o->velV = o->y.p.whole;
        o->velH = v;
        AnimLoadDuration(o);
        o->state = 1;
        break;
    case 1:
        func_800180D8_pcpx96084(o);
        o->y.raw += o->velY * 0x100;
        o->velY += 0x20;
        if (o->velY > 0) o->state = 2;
        break;
    case 2:
        func_800180D8_pcpx96084(o);
        o->y.raw += o->velY * 0x100;
        v = o->y.p.whole + 0x10;
        o->velY += 0x20;
        if (func_8001B9AC(o, o->h->p.whole, (short)(v - o->b0c * 16)) != 0) {
            o->active = 1;
            o->step = 1;
            o->state = 0;
            o->b69 = 0;
        }
        break;
    }
}
#endif
