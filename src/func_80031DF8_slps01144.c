// near-twin of PAL updateL4Type3EObject (tools/jp_autofix.py)
// VERSION SLPS_01144 slps01144 80031DF8 244
#if defined(VERSION_SLPS_01144)
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"
extern void ObjListPush_1F800224(TObj *);
extern void ObjCullRegister(TObj *);
extern void ObjFreeDup(TObj *);

void func_80031DF8(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        if (o->b0c == 0) {
            o->box0 = 0x50;
            o->box1 = 0xa0;
            o->box2 = 0x40;
            o->box3 = 0x5c;
        } else if (o->b0c == 1) {
            o->box0 = 0x50;
            o->box1 = 0xa0;
            o->box2 = 0x18;
            o->box3 = 0x24;
        }
        *(int *)((char *)o + 0xa0) = 0;
        break;
    case 1:
        if (o->b0c == 0) {
            o->visible = 1;
            ObjListPush_1F800224(o);
        } else {
            ObjCullRegister(o);
        }
        break;
    case 2:
        break;
    case 3:
        ObjFreeDup(o);
        break;
    }
}
#endif
#endif
