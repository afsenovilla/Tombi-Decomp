// FUNC 800f0984 260 I0NFO2
// MATCHING 800f0984 260
#include "TOBJ.H"
typedef struct {
    unsigned short w1e;
    short _p2;
    short spad;
    short _p6;
    void **anims;
} ObjInit3;
extern ObjInit3 TOPDOWN_A16_SCENERY_DEFS[];
extern int SPR_DATA[];
extern void func_8001FE6C(TObj *o);

void topdownA16SceneryInit(TObj *o)
{
    o->w1e = TOPDOWN_A16_SCENERY_DEFS[o->subtype].w1e;
    o->d3c = SPR_DATA[TOPDOWN_A16_SCENERY_DEFS[o->subtype].spad];
    o->b0d = 1;
    o->anim = TOPDOWN_A16_SCENERY_DEFS[o->subtype].anims[(unsigned short)o->wb4];
    func_8001FE6C(o);
    o->d8c = 0;
    if ((unsigned short)o->wb6 != 0) {
        o->b6b = 0;
        o->w08 = 0x7FC8;
    } else {
        o->b6b = 0x80;
        o->w08 = 0x7FC8;
    }
    *(signed char *)&o->b0f = -0xC;
    o->category |= 0x80;
    o->b04++;
}
