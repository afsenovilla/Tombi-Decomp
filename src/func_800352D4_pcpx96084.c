// VERSION PCPX_96084 tomba 800352D4 264
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 form of the SLPM-80165 pool type-17 updater (src/updateA00PoolType17.c): other draw constants, scratchpad word 0x1F80015C.
#include "TOBJ.H"
extern void *D_80013F38;
extern int D_1F80015C;
extern void func_80018550(TObj *o);
extern void func_8001A87C_pcpx96084(TObj *o);

void func_800352D4_pcpx96084(TObj *o)
{
    unsigned char b = o->b04;
    int v;
    void *p;
    switch (b) {
    case 0:
        switch (o->step) {
        case 0:
            v = D_1F80015C;
            p = D_80013F38;
            o->w1e = 7;
            o->b0f = 5;
            o->b0d = 0;
            *(short *)&o->b0a = 0;
            *((unsigned char *)o + 0x68) = 0;
            o->animFrame = 1;
            o->d3c = v;
            o->anim = p;
            o->step++;
            break;
        case 1:
            o->b04 = b + 1;
            o->step = 0;
            o->state = 0;
            o->substep = 0;
        }
        break;
    case 1:
        func_80018550(o);
        break;
    case 2:
        o->b04 = 3;
        break;
    case 3:
        func_8001A87C_pcpx96084(o);
    }
}
#endif
