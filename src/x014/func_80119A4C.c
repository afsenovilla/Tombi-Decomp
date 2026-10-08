// FUNC 80119a4c 268 X014
// MATCHING 80119a4c 268
#include "TOBJ.H"
extern int func_800205D8(int, int);
extern int FUN_800638cc(int);

int func_80119A4C(TObj *o, TObj *p)
{
    unsigned char a;
    int dx, dy;

    if (p == 0)
        goto fail;
    if (p->b6a == 0)
        return 0;
    a = 0x40;
    if ((p->animFrame & 2) == 0)
        a = ((p->animFrame & 1) == 0) << 7;
    if ((unsigned char)(a - o->d8c + 0x20) > 0x40)
        return 0;
    dx = p->h->p.whole - o->h->p.whole;
    dy = p->y.p.whole - o->y.p.whole;
    a = func_800205D8(dx, dy);
    o->wb0 = (unsigned char)a;
    o->d38 = a & 0xff;
    if ((unsigned char)(a - o->d8c + 0x20) > 0x40) {
    fail:
        return 0;
    }
    return FUN_800638cc(dx * dx + dy * dy) < 0x29;
}
