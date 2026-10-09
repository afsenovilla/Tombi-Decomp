// FUNC 8013759c 376 X001
// MATCHING 8013759c 376
#include "TOBJ.H"

extern int D_1F8002D4[];
extern void *D_8013E6D8[];
extern unsigned char D_8009CEAF;
extern int Rand(void);
extern void AnimLoadDuration(TObj *);

void func_8013759C(TObj *o)
{
    TObj *e;

    o->box0 = 10;
    o->box1 = 0x14;
    o->box2 = 10;
    o->box3 = 0x14;
    o->b0d = 0;
    o->b0a = 0;
    *(signed char *)&o->b0f = -10;
    o->animFrame = Rand() & 1;
    o->w1e = 8;
    o->d84 = 0;
    {
        int a = D_1F8002D4[0];
        void *b = D_8013E6D8[0];
        o->d3c = a;
        o->anim = b;
    }
    AnimLoadDuration(o);
    switch (o->subtype) {
    case 0:
        o->active = 1;
        o->b04 = 1;
        o->step = 0;
        o->state = 0;
        break;
    case 1:
        o->category |= 0x80;
        { unsigned char *c = &D_8009CEAF; *c = *c + 1; }
        e = (TObj *)o->d90;
        o->active = 2;
        o->b04 = 2;
        o->step = 3;
        *(signed char *)&o->b0f = -4;
        o->state = 0;
        o->d30 = o->h->raw - e->h->raw;
        o->d34 = o->y.raw - e->y.raw;
        break;
    case 2:
        e = (TObj *)o->d90;
        o->active = 2;
        o->b04 = 2;
        o->step = 4;
        o->state = 0;
        o->category |= 0x80;
        o->d30 = o->h->raw - e->h->raw;
        o->d34 = o->y.raw - e->y.raw;
        break;
    }
}
