// VERSION SLPS_01144 info 800E9E30 116
// JP version of PAL i0nfo 800e8398 (overheadNpcUpdateStep.c)
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"
extern void func_8002122C(TObj *o);
extern void func_80020E38(TObj *o);
void func_800E9E30(TObj *o)
{
    switch (o->step) {
    case 0:
        if (o->step == 0) {
            func_8002122C(o);
        } else {
    case 1: case 2: case 3: case 4: case 5:
            func_8002122C(o);
        }
        if (o->visible != 0) {
            func_80020E38(o);
        }
        break;
    case 6:
        break;
    }
}
#endif
