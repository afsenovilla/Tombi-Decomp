// FUNC 80119914 312 X014
// MATCHING 80119914 312
#include "TOBJ.H"

extern short D_8007A1F0[];
extern short COS_TABLE[];
extern int SquareRoot0(int);
extern int angleFromDelta(int, int);

int bossCapturePullBoss(TObj *o, TObj *t)
{
    int dx, dy, a, b;

    dx = t->h->p.whole - o->h->p.whole;
    dy = t->y.p.whole - o->y.p.whole;
    if (SquareRoot0(dx * dx + dy * dy) < 0x11) return 1;
    a = angleFromDelta(dx, dy) & 0xff;
    o->d38 = a;
    b = (a + 0x80) & 0xff;
    dy = (o->velH * D_8007A1F0[b]) >> 12;
    dx = (o->velH * COS_TABLE[b]) >> 12;
    t->h->raw += dx << 8;
    t->y.raw += dy << 8;
    o->velH -= 0x20;
    if (o->velH < 0x200) o->velH = 0x200;
    return 0;
}
