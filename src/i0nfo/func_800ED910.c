// FUNC 800ed910 312 I0NFO
// MATCHING 800ed910 312
#include "TOBJ.H"
typedef struct {
    unsigned char box[4];
    unsigned short w1e;
    short spad;
    void **anims;
} ObjInit;
extern ObjInit D_800EE90C[];
extern int D_1F8002C8[];
extern void func_8001FE6C(TObj *o);

void func_800ED910(TObj *o)
{
    o->box0 = D_800EE90C[o->subtype].box[0];
    o->box1 = D_800EE90C[o->subtype].box[1];
    o->box2 = D_800EE90C[o->subtype].box[2];
    o->box3 = D_800EE90C[o->subtype].box[3];
    o->w1e = D_800EE90C[o->subtype].w1e;
    o->d3c = D_1F8002C8[D_800EE90C[o->subtype].spad];
    o->anim = *D_800EE90C[o->subtype].anims;
    func_8001FE6C(o);
    o->b04++;
}
