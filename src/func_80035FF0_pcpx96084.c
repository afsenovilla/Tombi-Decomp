// VERSION PCPX_96084 tomba 80035FF0 208
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Timed object: drawn for 300 frames, then freed.
#include "TOBJ.H"
extern void ObjListPush_1F800230(TObj *);
extern void func_8001A87C_pcpx96084(TObj *);

void func_80035FF0_pcpx96084(TObj *o)
{
    unsigned char s = o->b04;
    switch (s) {
    case 0:
        o->b04 = s + 1;
        o->timer = 300;
        break;
    case 1:
        o->visible = 1;
        ObjListPush_1F800230(o);
        if (--o->timer == 0) o->b04++;
        break;
    case 2:
        o->b04 = s + 1;
        break;
    case 3:
        func_8001A87C_pcpx96084(o);
        break;
    }
}
#endif
