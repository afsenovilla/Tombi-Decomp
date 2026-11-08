// VERSION SLPS_01144 info3 800EF2B8 412
// JP version of PAL i0nfo3 800edce4 (tunnelActorInit.c)
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"
#include "raw7.h"
extern void func_80020DE4(TObj *o);
extern unsigned char D_8009E9DD[];
extern int SPR_DATA[];
typedef struct { unsigned char b0, b1, b2, b3; unsigned short w1e; short spad; void **anims; } EntD5C;
extern EntD5C D_800F0330[], D_800F0331[], D_800F0332[], D_800F0333[], D_800F0334[], D_800F0336[], D_800F0338[];

void func_800EF2B8(TObj *o)
{
    o->box0 = D_800F0330[o->subtype].b0;
    o->box1 = D_800F0331[o->subtype].b0;
    o->box2 = D_800F0332[o->subtype].b0;
    o->box3 = D_800F0333[o->subtype].b0;
    o->w1e = ((unsigned short *)&D_800F0334[o->subtype])[0];
    o->d3c = SPR_DATA[((short *)&D_800F0336[o->subtype])[0]];
    o->anim = ((void ***)&D_800F0338[o->subtype])[0][U16(o, 0xB4)];
    func_80020DE4(o);
    o->b6b = 0x7F;
    o->d8c = 0;
    o->b0d = 0;
    o->category |= 0x80;
    o->b04++;
    if (o->subtype == 0) {
        S8(o, 0xF) = -4;
    }
    if (D_8009E9DD[0] != 0) {
        int v = U16(o, 0xBA);
        if (v < 6) {
            if (v >= 4) {
                o->b04 = 3;
            }
        }
    }
}
#endif
