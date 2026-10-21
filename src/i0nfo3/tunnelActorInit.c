// FUNC 800edce4 404 I0NFO3
// MATCHING 800edce4 404
#include "TOBJ.H"
#include "raw7.h"
extern void func_8001FE6C(TObj *o);
extern unsigned char D_8009CE34[];
extern int D_1F8002C8[];
typedef struct { unsigned char b0, b1, b2, b3; unsigned short w1e; short spad; void **anims; } EntD5C;
extern EntD5C D_800EED5C[];

void tunnelActorInit(TObj *o)
{
    o->box0 = D_800EED5C[o->subtype].b0;
    o->box1 = D_800EED5C[o->subtype].b1;
    o->box2 = D_800EED5C[o->subtype].b2;
    o->box3 = D_800EED5C[o->subtype].b3;
    o->w1e = D_800EED5C[o->subtype].w1e;
    o->d3c = D_1F8002C8[D_800EED5C[o->subtype].spad];
    o->anim = D_800EED5C[o->subtype].anims[U16(o, 0xB4)];
    func_8001FE6C(o);
    o->b6b = 0x7F;
    S8(o, 0xF) = -4;
    o->d8c = 0;
    o->b0d = 0;
    o->category |= 0x80;
    o->b04++;
    if (D_8009CE34[0] != 0) {
        int v = U16(o, 0xBA);
        if (v < 6) {
            if (v >= 4) {
                o->b04 = 3;
            }
        }
    }
}
