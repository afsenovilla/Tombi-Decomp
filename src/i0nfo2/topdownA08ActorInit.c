// FUNC 800f16bc 516 I0NFO2
// MATCHING 800f16bc 516
#include "TOBJ.H"
#include "raw7.h"
extern void func_8001FE6C(TObj *o);
extern unsigned char D_8009CF01[];
extern int SPR_DATA[];
typedef struct { unsigned char b0, b1, b2, b3; unsigned short w1e; short spad; void **anims; } EntE58;
extern EntE58 TOPDOWN_A08_ACTOR_DEFS[];

void topdownA08ActorInit(TObj *o)
{
    o->box0 = TOPDOWN_A08_ACTOR_DEFS[o->subtype].b0;
    o->box1 = TOPDOWN_A08_ACTOR_DEFS[o->subtype].b1;
    o->box2 = TOPDOWN_A08_ACTOR_DEFS[o->subtype].b2;
    o->box3 = TOPDOWN_A08_ACTOR_DEFS[o->subtype].b3;
    o->w1e = TOPDOWN_A08_ACTOR_DEFS[o->subtype].w1e;
    o->d3c = SPR_DATA[TOPDOWN_A08_ACTOR_DEFS[o->subtype].spad];
    o->anim = TOPDOWN_A08_ACTOR_DEFS[o->subtype].anims[0];
    func_8001FE6C(o);
    o->d8c = 0;
    o->b0d = 0;
    o->category |= 0x80;
    o->b04++;
    switch (U16(o, 0xBA)) {
    case 1:
        o->anim = TOPDOWN_A08_ACTOR_DEFS[o->subtype].anims[7];
        func_8001FE6C(o);
        o->b0d = 1;
        o->w08 = 0x7FC8;
        o->b6b = 0x7F;
        if (D_8009CF01[0] != 0) {
            o->b04 = 3;
        }
        break;
    case 0:
        o->anim = TOPDOWN_A08_ACTOR_DEFS[o->subtype].anims[1];
        func_8001FE6C(o);
        o->b6b = 0x7F;
        break;
    default:
        o->b6b = 0;
        break;
    }
}
