// FUNC 800ea544 312 X006
// MATCHING 800ea544 312
#include "TOBJ.H"
extern TObj *FUN_80018448(void);
extern signed char DAT_80114a80[];

void FUN_800ea544(TObj *a, short x, short y, short z)
{
    TObj *o;
    signed char *p;
    int i;
    for (i = 0; i < 6; i++) {
        o = FUN_80018448();
        if (o != 0) {
            p = &DAT_80114a80[i * 4];
            o->active = 2;
            o->type = 5;
            o->animFrame = 0;
            o->a.raw = (x + *p++) << 16;
            o->y.raw = (y + *p++) << 16;
            o->b.raw = (z + *p++) << 16;
            o->wb8 = (signed char)*(unsigned char *)p;
            *(signed char *)&o->b0f = -2;
            o->b0d = 0;
            o->b0a = 2;
            o->subtype = i;
            o->d8c = 0x1000;
            o->d3c = a->d3c;
            o->w1e = a->w1e;
            o->b1d = a->b1d;
        }
    }
}
