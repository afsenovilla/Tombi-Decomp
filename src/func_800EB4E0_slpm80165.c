// VERSION SLPM_80165 x00 800EB4E0 228
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo version of func_8003CCB0_pcpx96084 (src/func_8003CCB0_pcpx96084.c): object whose init and per-frame steps
// come from two tables indexed by its byte +0x0C.
#include "TOBJ.H"
extern void (*D_8010EC44[])(TObj *);
extern void (*D_8010EC64[])(TObj *);
extern void func_8002099C(TObj *);
extern void func_800174B0(TObj *);

void func_800EB4E0(TObj *o)
{
    unsigned char s = o->b04;
    switch (s) {
    case 0:
        D_8010EC44[o->b0c](o);
        break;
    case 1:
        func_8002099C(o);
        if (o->visible) D_8010EC64[o->b0c](o);
        break;
    case 2:
        o->b04 = s + 1;
        break;
    case 3:
        func_800174B0(o);
        break;
    }
}
#endif
