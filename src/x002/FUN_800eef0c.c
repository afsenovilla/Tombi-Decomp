// FUNC 800eef0c 336 X002
// MATCHING 800eef0c 336
#include "TOBJ.H"
extern unsigned char DAT_8009c990;
extern unsigned short D_1f8001f8;
extern short D_1f800238;
extern TObj *FUN_80018448(void);
extern unsigned short D_800a604a[];
extern unsigned short TBL_801151e0[];

short FUN_800eef0c(void)
{
    short s = 0;
    TObj *o;
    switch (DAT_8009c990) {
    case 1:
        s = 5;
        break;
    case 2:
        s = (D_1f8001f8 & 1) + 5;
        break;
    case 3:
        s = 1;
        break;
    }
    if (s != 0) {
        if (D_1f800238 < 6) return 0;
        o = FUN_80018448();
        if (o != 0) {
            unsigned short *t;
            Fix16 *h = o->h;
            o->active = 1;
            o->type = 0x4e;
            o->subtype = DAT_8009c990 - 1;
            o->b0c = s;
            o->a.p.whole = D_800a604a[0];
            o->y.p.whole = D_800a604a[2];
            o->b.p.whole = D_800a604a[4];
            t = TBL_801151e0 + ((D_1f8001f8 + 2) & 7) * 2;
            h->p.whole += t[0];
            o->y.p.whole += t[1];
            o->step = 1;
        }
    }
    return s;
}
