// VERSION PCPX_96084 tomba 80043834 200
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Player fall step: wall contact and floor probe; with no contact flag set, falls with gravity 0x223 (a landing
// state 2 at speed 1095), else the vertical speed is cleared.
#include "TOBJ.H"
#define B(o, k) (((unsigned char *)(o))[k])
typedef struct { char pad[0x20]; short w20; } PX;
extern PX *D_800CCCCC;
extern void func_8004351C(TObj *);
extern void func_8001B704(TObj *, int);

void func_80043834_pcpx96084(TObj *o)
{
    o->ba5 = 0;
    func_8004351C(o);
    func_8001B704(o, 0);
    if ((o->b69 | B(o, 0x98) | B(o, 0x9a) | B(o, 0x9b) | B(o, 0xb6)) == 0) {
        o->ba5 = 1;
        o->velY += 0x223;
        o->y.raw += o->velY << 8;
        D_800CCCCC->w20 = 0x21;
        if (o->velY >= 1095) {
            o->ba5 = 0;
            o->velY = 0;
            B(o, 0x98) = 2;
            o->ba4 = 1;
        }
    } else {
        o->velY = 0;
        B(o, 0x98) = 0;
    }
}
#endif
