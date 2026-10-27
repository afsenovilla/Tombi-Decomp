// FUNC 800f126c 372 I0NFO2
// MATCHING 800f126c 372
#include "TOBJ.H"
typedef struct {
    unsigned short w1e;
    short _p2;
    short spad;
    short _p6;
    void **anims;
} ObjInit3;
extern ObjInit3 TOPDOWN_A17_SCENERY_DEFS[];
extern int SPR_DATA[];
extern void func_8001FE6C(TObj *o);
extern void topdownA17SceneryUpdate(TObj *o);
extern void func_800187E4(TObj *o);

void topdownA17SceneryHandler(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->w1e = TOPDOWN_A17_SCENERY_DEFS[o->subtype].w1e;
        o->d3c = SPR_DATA[TOPDOWN_A17_SCENERY_DEFS[o->subtype].spad];
        o->b0d = 1;
        o->anim = TOPDOWN_A17_SCENERY_DEFS[o->subtype].anims[(unsigned short)o->wb4];
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
        break;
    case 1:
        topdownA17SceneryUpdate(o);
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        func_800187E4(o);
        break;
    }
}
