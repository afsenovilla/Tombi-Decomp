// FUNC 8012176c 240 X014
// MATCHING 8012176c 240
#include "TOBJ.H"
extern TObj *FUN_800183b8(void);
extern int FUN_800205d8(int, int);
extern unsigned short D_1F80016A[], D_1F80016E[];

int func_8012176C(TObj *p, int sub)
{
    TObj *o = FUN_800183b8();
    short dx, dy;
    if (o != 0) {
        o->active = 1;
        o->type = 0x47;
        o->subtype = sub;
        o->b0c = 0;
        if (p->animFrame & 1) o->a.raw = (p->a.p.whole - 0x20) << 16;
        else o->a.raw = (p->a.p.whole + 0x20) << 16;
        o->y.raw = (p->y.p.whole - 4) << 16;
        o->b.raw = p->b.p.whole << 16;
        dx = D_1F80016A[0] - o->a.p.whole;
        dy = D_1F80016E[0] - o->y.p.whole;
        return o->d38 = (unsigned char)FUN_800205d8(dx, dy);
    }
}
