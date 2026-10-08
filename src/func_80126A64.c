// FUNC 80126a64 236 X000
// MATCHING 80126a64 236
#include "TOBJ.H"
typedef struct {
    TObj t;
    char pad[0xe8 - 0xc0];
    unsigned short e8;
    unsigned short ea;
} P80126A64;

void func_80126A64(P80126A64 *o, TObj *p)
{
    if (p->b0c == 0 && p->subtype != 2
        && (unsigned short)(o->t.d->p.whole - p->d->p.whole + 45) < 91
        && (unsigned short)(p->box0 + (o->e8 - p->h->p.whole)) < 19
        && (unsigned short)(p->box2 + (o->ea - p->y.p.whole + 2)) <= p->box3
        && !(o->t.animFrame & 1)) {
        short b;
        o->t.b9e = 2;
        b = p->box0;
        o->t.wba = 0x14;
        o->t.velY = 0;
        *(short *)0x1F80019E = 0;
        *(TObj **)0x1F8003C0 = p;
        o->t.wb8 = -b;
    }
}
