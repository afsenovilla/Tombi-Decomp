// FUNC 800f35e0 188 X000
#include "TOBJ.H"
extern char *DAT_8009c330;
extern unsigned char DAT_801152e8[];
extern void FUN_8001e5f4(int, int);
extern void FUN_8001e4f0(int);
extern void FUN_800eae0c(int, short, int, int);

void FUN_800f35e0(TObj *o)
{
    FUN_8001e5f4(0x1c, 0x7f);
    DAT_8009c330[8] = 0;
    o->step = 3;
    o->state = 1;
    o->ba7 = 0;
    *(char *)&o->wac = 0;
    o->b9c = 0;
    o->d8c = DAT_801152e8[o->wb0];
    if (o->bbe & 0x20) {
        FUN_8001e4f0(0x3e);
        FUN_800eae0c(o->h->p.whole, o->y.p.whole, o->d->p.whole, 0);
        FUN_800eae0c(o->h->p.whole, o->y.p.whole, o->d->p.whole, 0);
    }
}
