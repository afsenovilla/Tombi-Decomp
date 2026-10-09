// FUNC 801293a0 388 X004
// MATCHING 801293a0 388
#include "TOBJ.H"
typedef struct {
    unsigned char b0, b1, b2, b3;
    unsigned short w1e;
    short idx;
    void **tab;
} Ent;
extern Ent D_80131174[];
extern int D_1F8002C8[];
extern unsigned char D_8009CE22;
extern unsigned char D_800A6047[];
extern void FUN_8001fe6c(TObj *);

void func_801293A0(TObj *o)
{
    if (D_8009CE22) {
        o->box0 = D_80131174[o->subtype].b0;
        o->box1 = D_80131174[o->subtype].b1;
        o->box2 = D_80131174[o->subtype].b2;
        o->box3 = D_80131174[o->subtype].b3;
        o->w1e = D_80131174[o->subtype].w1e;
        o->d3c = D_1F8002C8[D_80131174[o->subtype].idx];
        o->anim = *D_80131174[o->subtype].tab;
        FUN_8001fe6c(o);
        o->b0d = 1;
        o->w08 = 0x7809;
        o->d8c = 0;
        o->b0a = 2;
        o->b68 = 0;
        o->category |= 0x80;
        o->b0f = D_800A6047[0] + 5;
    }
    o->b04++;
}
