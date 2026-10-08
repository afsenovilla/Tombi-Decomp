// FUNC 80118974 372 X016
// MATCHING 80118974 372
#include "TOBJ.H"

typedef struct {
    unsigned char b0, b1, b2, b3;
    unsigned short w4;
    short w6;
    void **anim;
} T80118974;

extern T80118974 D_80118D2C[];
extern int D_1F8002C8[];
extern unsigned char D_8009CDED;
extern void AnimLoadDuration(TObj *);

void func_80118974(TObj *o)
{
    o->box0 = D_80118D2C[o->subtype].b0;
    o->box1 = D_80118D2C[o->subtype].b1;
    o->box2 = D_80118D2C[o->subtype].b2;
    o->box3 = D_80118D2C[o->subtype].b3;
    o->w1e = D_80118D2C[o->subtype].w4;
    o->d3c = D_1F8002C8[D_80118D2C[o->subtype].w6];
    o->anim = D_80118D2C[o->subtype].anim[(unsigned short)o->wb4];
    AnimLoadDuration(o);
    o->d8c = 0;
    o->b0d = 0;
    o->b0a = 0;
    o->b0f = 0;
    o->step = 0;
    o->category |= 0x80;
    o->b04++;
    if (D_8009CDED == 0xff) o->b04 = 3;
}
