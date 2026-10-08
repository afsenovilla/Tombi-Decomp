// FUNC 80118998 296 X001
// MATCHING 80118998 296
#include "TOBJ.H"
extern short D_8007A5F0[];
extern short D_8007A3F0[];
extern TObj *FUN_80018448(void);

void func_80118998(short x, short y, int z)
{
    int i;
    TObj *e;
    int w;
    int j;

    for (i = 0; i < 32; i++) {
        e = FUN_80018448();
        if (e) {
            j = i >> 1;
            e->type = 0xe;
            e->active = 1;
            e->h->raw = x << 16;
            e->y.raw = y << 16;
            e->d->raw = z << 16;
            e->timer = 0x40;
            w = (i & 1) << 3;
            e->subtype = 1;
            e->b0c = 0;
            e->w22 = w;
            if (j & 1) e->w22 = w + 8;
            e->d84 = j << 4;
            e->animFrame = 1;
            e->d88 = 0;
            e->h->raw += D_8007A5F0[e->d84] << 5;
            e->y.raw += D_8007A3F0[e->d84] << 5;
        }
    }
}
