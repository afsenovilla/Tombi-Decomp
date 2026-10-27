// FUNC 800e9358 184 I0NFO2
// MATCHING 800e9358 184
#include "TOBJ.H"
typedef struct {
    unsigned short w1e;
    short spad;
    void **anims;
    int pad;
} ObjInit2;
extern ObjInit2 TOPDOWN_SHADOW_DEFS[];
extern int SPR_DATA[];
extern void func_8001FE6C(TObj *o);

void topdownShadowInit(TObj *o)
{
    o->w1e = TOPDOWN_SHADOW_DEFS[o->subtype].w1e;
    o->d3c = SPR_DATA[TOPDOWN_SHADOW_DEFS[o->subtype].spad];
    o->anim = TOPDOWN_SHADOW_DEFS[o->subtype].anims[32];
    func_8001FE6C(o);
    o->b04++;
}
