// FUNC 80126e8c 216 X001
// MATCHING 80126e8c 216
#include "TOBJ.H"
typedef struct {
    TObj t;
    char pad[0xe8 - 0xc0];
    unsigned short e8;
    unsigned short ea;
} P;
extern short D_1F80019E;
extern TObj *D_1F8003C0;

void func_80126E8C(P *o, TObj *e)
{
    if (e->subtype == 6
        && (unsigned short)(o->t.d->p.whole - e->d38 + 45) < 91
        && (unsigned short)(e->box0 + (o->e8 - e->d30)) <= e->box1
        && (unsigned short)(e->box2 + (o->ea - e->d34)) <= e->box3) {
        o->t.b9e = 3;
        o->t.wb8 = e->d30 - e->h->p.whole;
        o->t.wba = e->d34 - e->y.p.whole;
        o->t.velY = 0;
        e->b69 = 1;
        D_1F80019E = 0;
        D_1F8003C0 = e;
    }
}
