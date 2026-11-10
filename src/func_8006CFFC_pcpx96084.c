// VERSION PCPX_96084 tomba 8006CFFC 124
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 version of PAL tryAttachObjectOnResult3 (src/tryAttachObjectOnResult3.c): flag +0xA4, link +0xB8.
#include "TOBJ.H"
short func_80069924(TObj *o, TObj *e);

void tryAttachObjectOnResult3(TObj *o, TObj *e)
{
    short r = func_80069924(o, e);
    if (r != -1 && r == 3 && o->ba4 == 1) {
        e->active = 2;
        e->b04 = 2;
        e->step = 0;
        e->state = 0;
        e->b69 = 0;
        *(TObj **)((char *)o + 0xb8) = e;
        o->ba4 = 2;
    }
}
#endif
