// VERSION PCPX_96084 tomba 8003E284 220
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Object handler: boxes cleared on start (subtype 0 only), then culled with the larger of the +0x6C / +0x70 extents.
#include "TOBJ.H"
extern void func_800183FC(TObj *o, int n);
extern void func_8001A8C8_pcpx96084(TObj *o);

void func_8003E284_pcpx96084(TObj *o)
{
    char pad[1];
    unsigned char s = o->b04;
    switch (s) {
    case 0:
        if (o->subtype == 0) {
            o->box0 = 0;
            o->box1 = 0;
            o->box2 = 0;
            o->box3 = 0;
        }
        o->b04++;
        break;
    case 1:
        {
            short m = o->box0;
            if (o->box0 < o->box2) m = o->box2;
            func_800183FC(o, m);
        }
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
