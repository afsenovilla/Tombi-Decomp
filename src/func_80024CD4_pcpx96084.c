// VERSION PCPX_96084 tomba 80024CD4 216
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Scene object: no collision box, category bit 0x80, stepped by func_80024DAC and drawn through list 0x1F80022C.
#include "TOBJ.H"
extern void func_80024DAC(TObj *);
extern void ObjListPush_1F80022C(TObj *);
extern void func_8001A914_pcpx96084(TObj *);

void func_80024CD4_pcpx96084(TObj *o)
{
    unsigned char s = o->b04;
    switch (s) {
    case 0:
        *(short *)&o->b0a = 0x11;
        ((unsigned char *)o)[0xa0] = 1;
        o->box0 = 0;
        o->box1 = 0;
        o->box2 = 0;
        o->box3 = 0;
        o->category |= 0x80;
        o->b04++;
        break;
    case 1:
        func_80024DAC(o);
        o->visible = 1;
        ObjListPush_1F80022C(o);
        break;
    case 2:
        o->b04 = s + 1;
        break;
    case 3:
        func_8001A914_pcpx96084(o);
        break;
    }
}
#endif
