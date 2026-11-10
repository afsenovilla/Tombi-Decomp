// VERSION PCPX_96084 tomba 8003C014 280
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Object whose init and per-frame steps come from two tables indexed by its byte +0x0C (D_800A7AE4 / D_800A7AF0);
// the step runs only on screen and left of x 1601.
#include "TOBJ.H"
extern void (*D_800A7AE4[])(TObj *);
extern void (*D_800A7AF0[])(TObj *);
extern void func_80018550(TObj *);
extern void func_8001A8C8_pcpx96084(TObj *);

void func_8003C014_pcpx96084(TObj *o)
{
    unsigned char s = o->b04;
    switch (s) {
    case 0:
        D_800A7AE4[o->b0c](o);
        o->state = 0;
        o->b04++;
        break;
    case 1:
        func_80018550(o);
        if (o->visible && o->h->p.whole < 1601) D_800A7AF0[o->b0c](o);
        break;
    case 2:
        o->b04 = s + 1;
        break;
    case 3:
        func_8001A8C8_pcpx96084(o);
        break;
    }
}
#endif
