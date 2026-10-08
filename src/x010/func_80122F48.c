// FUNC 80122f48 392 X010
// MATCHING 80122f48 392
#include "TOBJ.H"
extern int D_1F8002D4[];
extern void *D_80131DBC;
extern unsigned char D_8009CEAF;
extern int Rand(void);
extern void AnimLoadDuration(TObj *);

void func_80122F48(TObj *o)
{
    TObj *p;
    o->box0 = 10;
    o->box1 = 0x14;
    o->box2 = 10;
    o->box3 = 0x14;
    o->b0d = 1;
    o->w08 = 0x7f10;
    o->b0a = 0;
    *(signed char *)&o->b0f = -10;
    o->animFrame = Rand() & 1;
    o->w1e = 0xe;
    o->d84 = 0;
    o->d3c = D_1F8002D4[0];
    o->anim = D_80131DBC;
    AnimLoadDuration(o);
    switch (o->subtype) {
    case 0:
        o->active = 1;
        o->b04 = 1;
        o->step = 0;
        o->state = 0;
        *(signed char *)&o->b0f = -4;
        break;
    case 1:
        o->category |= 0x80;
        { unsigned char *c = &D_8009CEAF; *c = *c + 1; }
        p = (TObj *)o->d90;
        o->active = 2;
        o->b04 = 2;
        o->step = 3;
        o->state = 0;
        o->d30 = o->h->raw - p->h->raw;
        o->d34 = o->y.raw - p->y.raw;
        break;
    case 2:
        p = (TObj *)o->d90;
        o->active = 2;
        o->b04 = 2;
        o->step = 4;
        o->state = 0;
        o->category |= 0x80;
        o->d30 = o->h->raw - p->h->raw;
        o->d34 = o->y.raw - p->y.raw;
        break;
    }
}
