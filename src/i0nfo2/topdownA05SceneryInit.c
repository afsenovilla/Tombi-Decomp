// FUNC 800f0658 260 I0NFO2
// MATCHING 800f0658 260
#include "TOBJ.H"
typedef struct {
    unsigned short w1e;
    short _p2;
    short spad;
    short _p6;
    void **anims;
} ObjInit3;
extern ObjInit3 D_800FEDC4[];
extern int D_1F8002C8[];
extern void func_8001FE6C(TObj *o);

void topdownA05SceneryInit(TObj *o)
{
    o->w1e = D_800FEDC4[o->subtype].w1e;
    o->d3c = D_1F8002C8[D_800FEDC4[o->subtype].spad];
    o->b0d = 1;
    o->anim = D_800FEDC4[o->subtype].anims[(unsigned short)o->wb4];
    func_8001FE6C(o);
    o->d8c = 0;
    if ((unsigned short)o->wb6 != 0) {
        o->b6b = 0;
        o->w08 = 0x7F48;
    } else {
        o->b6b = 0x80;
        o->w08 = 0x7FC8;
    }
    *(signed char *)&o->b0f = -0xC;
    o->category |= 0x80;
    o->b04++;
}
