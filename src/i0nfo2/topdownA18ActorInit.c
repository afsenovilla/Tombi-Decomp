// FUNC 800f5334 344 I0NFO2
// MATCHING 800f5334 344
#include "TOBJ.H"
typedef struct {
    unsigned char box[4];
    unsigned short w1e;
    short spad;
    void **anims;
} ObjInit;
extern ObjInit D_800FEFA0[];
extern int D_1F8002C8[];
extern void func_8001FE6C(TObj *o);

void topdownA18ActorInit(TObj *o)
{
    o->box0 = D_800FEFA0[o->subtype].box[0];
    o->box1 = D_800FEFA0[o->subtype].box[1];
    o->box2 = D_800FEFA0[o->subtype].box[2];
    o->box3 = D_800FEFA0[o->subtype].box[3];
    o->w1e = D_800FEFA0[o->subtype].w1e;
    o->d3c = D_1F8002C8[D_800FEFA0[o->subtype].spad];
    o->anim = D_800FEFA0[o->subtype].anims[(unsigned short)o->wb4];
    func_8001FE6C(o);
    o->b6b = 0x7F;
    o->d8c = 0;
    o->b0d = 0;
    o->category |= 0x80;
    o->b04++;
}
