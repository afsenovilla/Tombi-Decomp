// FUNC 80119864 292 X000
#include "TOBJ.H"
struct V3 { int x, y, z; };
extern short FUN_8001fe0c(int, int);
extern short FUN_8001fe3c(int, int);

void FUN_80119864(TObj *o)
{
    TObj *p;
    int r;

    p = (TObj *)o->d90;
    *(struct V3 *)&o->a = *(struct V3 *)&p->a;
    r = FUN_8001fe0c(o->wb4, *(short *)&o[1].active);
    o->h->raw = o->h->raw + r * 0x10000;
    r = FUN_8001fe3c(o->wb6, *(short *)&o[1].type);
    o->y.raw = o->y.raw + r * 0x10000;
    r = FUN_8001fe3c(o->wb8, *(short *)&o[1].b04);
    o->d->raw = o->d->raw + r * 0x10000;
    o->wb4 = (o->wb4 + o->wba) & 0xff;
    o->wb6 = (o->wb6 + o->wbc) & 0xff;
    o->wb8 = (o->wb8 + *(short *)&o->bbe) & 0xff;
    o->d64 = p->d64;
    if (p->b04 > 1)
        o->b04 = o->b04 + 1;
}
