// VERSION PCPX_96084 tomba 80036948 196
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 version of PAL x018 updateJuiceManJoinFight: the init state steps here, the talk runs while visible.
#include "TOBJ.H"
extern void func_800361F8(TObj *);
extern void func_800363E0(TObj *);
extern int func_80018550(TObj *);
extern void func_8001A87C_pcpx96084(TObj *);

void updateJuiceManJoinFight(TObj *o)
{
    switch (o->b04) {
    case 0:
        func_800361F8(o);
        o->b04++;
        break;
    case 1:
        func_80018550(o);
        if (o->visible != 0) func_800363E0(o);
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        func_8001A87C_pcpx96084(o);
        break;
    }
}
#endif
