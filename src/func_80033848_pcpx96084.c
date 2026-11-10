// VERSION PCPX_96084 tomba 80033848 212
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Scene object: animation start, then a per-frame step (subtype 0 only), animation advance and a draw-list push.
#include "TOBJ.H"
extern void AnimLoadDuration(TObj *o);
extern void func_8003391C(TObj *o);
extern void func_800180D8_pcpx96084(TObj *o);
extern void ObjListPush_1F800230(TObj *o);
extern void func_8001A87C_pcpx96084(TObj *o);

void func_80033848_pcpx96084(TObj *o)
{
    unsigned char b = o->b04;
    switch (b) {
    case 0:
        o->timer = 0;
        AnimLoadDuration(o);
        o->step = 0;
        o->b04++;
        break;
    case 1:
        if (o->subtype == 0) func_8003391C(o);
        func_800180D8_pcpx96084(o);
        o->visible = 1;
        ObjListPush_1F800230(o);
        break;
    case 2:
        o->b04 = b + 1;
        break;
    case 3:
        func_8001A87C_pcpx96084(o);
        break;
    }
}
#endif
