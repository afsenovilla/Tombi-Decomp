// VERSION PCPX_96084 tomba 8003E494 256
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Spinning scenery: set up by func_8003E360, then turns +0x8C by +8 (subtypes 0, 3, 8) or -8 (1, 4, 7, 9) a frame.
#include "TOBJ.H"
extern void func_8003E360(TObj *);
extern void func_80018550(TObj *);
extern void func_8001A8C8_pcpx96084(TObj *);

void func_8003E494_pcpx96084(TObj *o)
{
    unsigned char s = o->b04;
    switch (s) {
    case 0:
        func_8003E360(o);
        break;
    case 1:
        func_80018550(o);
        switch (o->subtype) {
        case 0:
            o->d8c = (o->d8c + 8) & 0xfff;
            break;
        case 3:
            o->d8c = (o->d8c + 8) & 0xfff;
            break;
        case 8:
            o->d8c = (o->d8c + 8) & 0xfff;
            break;
        case 1:
        case 4:
        case 7:
        case 9:
            o->d8c = (o->d8c - 8) & 0xfff;
            break;
        case 2:
        case 5:
        case 6:
            break;
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
