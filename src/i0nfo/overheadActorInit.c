// FUNC 800ed7b0 352 I0NFO
// MATCHING 800ed7b0 352
#include "TOBJ.H"
typedef struct {
    unsigned char box[4];
    unsigned short w1e;
    short spad;
    void **anims;
} ObjInit;
extern ObjInit D_800EE90C[];
extern int D_1F8002C8[];
extern void func_8001FE94(TObj *o, int n);
extern void func_800202B4(TObj *o);

void overheadActorInit(TObj *o)
{
    o->box0 = D_800EE90C[o->subtype].box[0];
    o->box1 = D_800EE90C[o->subtype].box[1];
    o->box2 = D_800EE90C[o->subtype].box[2];
    o->box3 = D_800EE90C[o->subtype].box[3];
    o->w1e = D_800EE90C[o->subtype].w1e;
    o->d3c = D_1F8002C8[D_800EE90C[o->subtype].spad];
    o->anim = D_800EE90C[o->subtype].anims[o->w74];
    func_8001FE94(o, o->w76);
    o->w08 = 0x7809;
    o->d8c = 0;
    o->b0d = 0;
    o->category |= 0x80;
    o->b04++;
    func_800202B4(o);
}
