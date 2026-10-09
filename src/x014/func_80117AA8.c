// FUNC 80117aa8 424 X014
// MATCHING 80117aa8 424
#include "TOBJ.H"
typedef struct { short w0; short w2; } SB;

extern int ObjCullRegister(TObj *);

#define I32(p, off) (*(int *)((char *)(p) + (off)))

void func_80117AA8(TObj *o)
{
    TObj *p = (TObj *)o->d90;
    SB *s, *t;

    o->h->p.whole = p->h->p.whole + I32(p, 0x5c);
    o->y.p.whole = p->y.p.whole + p->d60;
    o->d->p.whole = p->d->p.whole + p->d64;
    o->animFrame = (o->animFrame + 1) & 0xff;
    o->d8c = o->animFrame << 4;
    s = (SB *)&o->wb4;
    t = (SB *)&o->wbc;
    switch (o->step) {
    case 0:
        if (--o->timer == -1) o->step++;
        break;
    case 1:
        s->w2 -= 4;
        t->w2 -= 4;
        if (s->w2 <= o->w76) {
            o->timer = 0x40;
            o->step++;
        }
        break;
    case 2:
        if (--o->timer == -1) o->step++;
        break;
    case 3:
        if (s->w2 < 0) {
            s->w2 += 4;
            t->w2 += 4;
        }
        o->d30 -= 4;
        if (o->d30 <= 0) {
            o->d30 = 0;
            o->step++;
        }
        break;
    case 4:
        o->b04 = 3;
        break;
    }
    ObjCullRegister(o);
}
