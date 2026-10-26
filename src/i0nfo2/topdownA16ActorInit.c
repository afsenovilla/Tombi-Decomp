// FUNC 800f3c28 348 I0NFO2
// MATCHING 800f3c28 348
#include "TOBJ.H"
typedef struct {
    unsigned char box[4];
    unsigned short w1e;
    short spad;
    void **anims;
} ObjInit;
extern ObjInit D_800FEEF4[];
extern int SPR_DATA[];
extern void func_8001FE94(TObj *o, int n);

void topdownA16ActorInit(TObj *o)
{
    o->box0 = D_800FEEF4[o->subtype].box[0];
    o->box1 = D_800FEEF4[o->subtype].box[1];
    o->box2 = D_800FEEF4[o->subtype].box[2];
    o->box3 = D_800FEEF4[o->subtype].box[3];
    o->w1e = D_800FEEF4[o->subtype].w1e;
    o->d3c = SPR_DATA[D_800FEEF4[o->subtype].spad];
    o->anim = D_800FEEF4[o->subtype].anims[o->animFrame];
    func_8001FE94(o, 0);
    o->b6b = 0x7F;
    o->d8c = 0;
    o->b0d = 0;
    o->category |= 0x80;
    o->b04++;
}
