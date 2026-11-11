// VERSION PCPX_96084 tomba 80032D60 772
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 form of PAL x000 updateA00FallingLeaf (src/updateA00FallingLeaf.c): halfword at +0x0A, scratchpad word 0x1F80015C, states advance by one; the volatile store keeps the anim store before the scratchpad load.
extern int D_1F80015C;
#define D_1F80015C_ ((int *)&D_1F80015C)
#define D_1F80015C_ ((int *)&D_1F80015C)
#define D_1F80015C_ ((int *)&D_1F80015C)
#include "spad.h"
#include "TOBJ.H"
extern void *D_80014028[];
extern int Rand(void);
extern void AnimLoadDuration(TObj *);
extern int func_80018550(TObj *);
extern void func_8001A87C_pcpx96084(TObj *);
void func_80032D60_pcpx96084(TObj *o)
{
    short f;
    switch (o->b04) {
    case 0:
        if (o->timer == 0) {
        o->b04++;
        o->w1e = 9;
        *(short *)&o->b0a = 3;
        o->d64 = 0xac8;
        o->b0d = 0;
        *(signed char *)&o->b0f = -10;
        *(void * volatile *)&o->anim = D_80014028[o->b0c];
        o->d3c = D_1F80015C;
        o->animFrame = Rand() & 1;
        switch (o->subtype) {
        case 0:
            o->d8c = 0x40;
            o->d84 = -2;
            o->velH = 0x100;
            o->velX = -8;
            o->velV = 0x100;
            break;
        case 1:
            o->d8c = -0x40;
            o->d84 = 2;
            o->velH = -0x100;
            o->velX = 8;
            o->velV = 0xc0;
            break;
        case 2:
            o->d8c = -0x60;
            o->d84 = 2;
            o->velH = 0x100;
            o->velX = -8;
            o->velV = 0x80;
            break;
        case 3:
            o->d8c = 0x60;
            o->d84 = -2;
            o->velH = -0x100;
            o->velX = 8;
            o->velV = 0x80;
            break;
        }
        AnimLoadDuration(o);
        } else {
            o->timer--;
        }
        break;
    case 1:
        if (func_80018550(o) == 0) {
            o->b04++;
            break;
        }
        o->y.raw += o->velV << 8;
        o->h->raw += o->velH << 8;
        o->velH += o->velX;
        {
            int vx = o->velX;
            if (vx < 0 && o->velH < -0xff)
                o->velX = -vx;
            else {
                int w = o->velX;
                if (w > 0 && o->velH > 0xff)
                    o->velX = -w;
            }
        }
        o->d8c += o->d84;
        switch (o->subtype) {
        case 0 ... 1:
            f = (unsigned)(o->d8c + 0x3f) < 0x7f;
            goto tail;
        case 2 ... 3:
            f = (unsigned)(o->d8c + 0x5f) < 0xbf;
        tail:
            if (!f)
                o->d84 = -o->d84;
            break;
        }
        if (++o->timer >= 0x78)
            o->b04++;
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
