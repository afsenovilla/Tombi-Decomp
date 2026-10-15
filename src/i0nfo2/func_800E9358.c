// FUNC 800e9358 184 I0NFO2
// MATCHING 800e9358 184
#include "TOBJ.H"
typedef struct {
    unsigned short w1e;
    short spad;
    void **anims;
    int pad;
} ObjInit2;
extern ObjInit2 D_800FEBD4[];
extern int D_1F8002C8[];
extern void func_8001FE6C(TObj *o);

void func_800E9358(TObj *o)
{
    o->w1e = D_800FEBD4[o->subtype].w1e;
    o->d3c = D_1F8002C8[D_800FEBD4[o->subtype].spad];
    o->anim = D_800FEBD4[o->subtype].anims[32];
    func_8001FE6C(o);
    o->b04++;
}
