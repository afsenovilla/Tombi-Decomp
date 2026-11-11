// VERSION PCPX_96084 tomba 80040E24 252
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Background layer scroll by kind (+0x0C): kinds 0/1 drift left with wrap-around, kinds 2/3 follow the word +0x30
// at 1/16 and 1/8; the height follows +0x34 against the screen origin; drawn through func_8001AC5C_pcpx96084.
#include "TOBJ.H"
extern unsigned short D_1F800392;
extern void func_8001AC5C_pcpx96084(TObj *);

void func_80040E24_pcpx96084(TObj *o_)
{
    register TObj *o asm("$5") = o_;
    switch (o->b0c) {
    case 0:
        if (o->d30 < (int)0xffc40000) o->d30 = 0x17c0000;
        goto drift;
    case 1:
        if (o->d30 < (int)0xffd80000) o->d30 = 0x1680000;
    drift:
        o->d30 -= o->animFrame << 4;
        o->a.raw = o->d30;
        break;
    case 2:
        o->a.p.whole = (unsigned int)o->d30 >> 4;
        break;
    case 3:
        o->a.p.whole = (unsigned int)o->d30 >> 3;
        break;
    }
    { int d = o->d34; int s = D_1F800392;
    o->visible = 1;
    o->y.p.whole = ((unsigned)(d - s) & 0xfff) >> 4; }
    func_8001AC5C_pcpx96084(o);
}

#endif
