// VERSION PCPX_96084 tomba 80041F90 344
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 form of playerSelectGroundAnim (src/playerSelectGroundAnim.c): speed at +0xAA, pad bit 0x80, no subtype override.
#include "TOBJ.H"
typedef struct { char pad[0x2c]; unsigned short cur; unsigned short prev; } Cam2c;
extern Cam2c *D_800CCCCC;
extern int D_800DE0FC;
extern unsigned short D_800DE760;
extern unsigned short BUTTON_MASK_A;
extern void func_80041CE4(TObj *o, int v);

void func_80041F90_pcpx96084(TObj *o)
{
    short v;
    if (o->waa == 0) {
        D_800CCCCC->cur = 0;
    } else {
        if ((D_800DE0FC & 0x40) && (D_800DE760 & 0x80) && o->b04 == 1) {
            D_800CCCCC->cur = 0x21;
        } else {
            v = o->waa;
            if (v < -0x143)
                D_800CCCCC->cur = 3;
            else if (v < -0x84)
                D_800CCCCC->cur = 2;
            else if (v < 0)
                D_800CCCCC->cur = 1;
            else if (v >= 0x144)
                D_800CCCCC->cur = 3;
            else if (v >= 0x85)
                D_800CCCCC->cur = 2;
            else if (v > 0)
                D_800CCCCC->cur = 1;
        }
    }
    if (D_800CCCCC->prev != D_800CCCCC->cur) {
        func_80041CE4(o, D_800CCCCC->cur);
        D_800CCCCC->prev = D_800CCCCC->cur;
    }
}
#endif
