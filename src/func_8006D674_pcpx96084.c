// VERSION PCPX_96084 tomba 8006D674 132
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 version of PAL tryAttachObjectOnOverlap (src/tryAttachObjectOnOverlap.c): flag +0xA4, link +0xB8, scratchpad 0x1F8002A6.
#include "spad.h"
#include "TOBJ.H"
typedef struct { TObj o; } TObjX;
#define E_B8(o) (*(TObj **)((char *)(o) + 0xb8))
extern short func_80069700(TObjX *o, TObj *p);

void tryAttachObjectOnOverlap(TObjX *o, TObj *p)
{
    short r = func_80069700(o, p);
    if (r != 0) {
        if (o->o.ba4 == 1 && r == 1) {
            p->active = 2;
            p->b04 = 2;
            p->step = 1;
            p->state = 0;
            p->b69 = 0;
            E_B8(o) = p;
            o->o.ba4 = 2;
        }
        ((short *)&SPAD)[0x2A6 / 2] = 0;
    }
}
#endif
