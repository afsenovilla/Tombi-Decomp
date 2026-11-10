// VERSION PCPX_96084 tomba 8003CC2C 132
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 version of JP updateKeyHoles (src/x004/updateKeyHoles.c): states 0 and 2 step to the next state.
#include "TOBJ.H"
extern void func_80018550(TObj *);
extern void func_8001A8C8_pcpx96084(TObj *);

void updateKeyHoles(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        break;
    case 1:
        func_80018550(o);
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        func_8001A8C8_pcpx96084(o);
        break;
    }
}
#endif
