// FUNC 80124410 144 X001
// MATCHING 80124410 144
#include "TOBJ.H"

typedef struct {
    TObj o;
    unsigned char pad[0x24];
    TObj *e4;
} P;

extern short objectBoxOverlapSideFacing(P *, TObj *);
extern void spawnImpactEffect(int, short, short, short);

void grabObjectContact(P *a, TObj *b)
{
    short r = objectBoxOverlapSideFacing(a, b);

    if (r != -1 && r < 3 && *(unsigned char *)&a->o.wac == 1) {
        b->active = 2;
        b->b04 = 2;
        b->step = 0;
        b->state = 0;
        b->b69 = 0;
        a->e4 = b;
        *(unsigned char *)&a->o.wac = 2;
        spawnImpactEffect(2, a->o.a.p.whole, a->o.y.p.whole, a->o.b.p.whole);
    }
}
