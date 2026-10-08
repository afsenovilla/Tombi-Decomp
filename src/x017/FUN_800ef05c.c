// FUNC 800ef05c 244 X017
// MATCHING 800ef05c 244
#include "TOBJ.H"
extern short FUN_800eef0c(void);
extern TObj *FUN_80018448(void);
extern short DAT_1f800238;
extern unsigned short DAT_1f8001f8;
extern short DAT_801151e0[];
void FUN_800ef05c(unsigned int a, short x, short y, short z)
{
    TObj *p;
    short *t;
    if (FUN_800eef0c() == 0 && DAT_1f800238 > 5 && (p = FUN_80018448()) != 0) {
        p->active = 1;
        p->type = 0x31;
        p->subtype = 1;
        p->a.p.whole = x;
        p->y.p.whole = y;
        p->b.p.whole = z;
        t = DAT_801151e0 + (a & 1) * 16 + (DAT_1f8001f8 & 7) * 2;
        p->h->p.whole += t[0];
        p->y.p.whole += t[1];
    }
}
