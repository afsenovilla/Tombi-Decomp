// FUNC 80126d18 268 X003
// MATCHING 80126d18 268
#include "TOBJ.H"

extern unsigned char D_80135CB0[];
extern void *D_80139578;
extern short D_1F80027E;
extern void AnimLoadDuration(TObj *);
extern short FUN_800408d8(TObj *, short, short);

void func_80126D18(TObj *o)
{
    unsigned char *p;

    switch (o->state) {
    case 0:
        o->state++;
        o->wac = 0x1e;
        o->anim = D_80139578;
        p = D_80135CB0 + (o->wac << 2);
        o->box0 = *p++;
        o->box1 = *p++;
        o->box2 = *p;
        o->box3 = p[1];
        AnimLoadDuration(o);
        break;
    case 1:
        o->y.p.whole -= 4;
        if (FUN_800408d8(o, o->h->p.whole, o->y.p.whole - 0x10) != 0) {
            o->d8c = (-D_1F80027E * 4 + 0x80) & 0xff;
            o->state++;
        }
        break;
    case 2:
        break;
    }
}
