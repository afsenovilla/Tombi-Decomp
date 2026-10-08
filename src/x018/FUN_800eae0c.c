// FUNC 800eae0c 496 X018
// MATCHING 800eae0c 496
#include "TOBJ.H"
extern unsigned short DAT_8009c960;
extern unsigned short DAT_8009c962;
extern unsigned char DAT_8009d2c3;
extern unsigned char DAT_8009c933;
extern unsigned char DAT_8009ceaf;
extern unsigned char DAT_8009d0b4;
extern unsigned char DAT_8009cf0a;
extern unsigned char DAT_8009c959;
extern TObj *FUN_80018448(void);
extern unsigned short FUN_8001f9e0(void);
extern void FUN_80114968(TObj *, int, int, int, int);

void FUN_800eae0c(short x, short y, short z, unsigned char f)
{
    TObj *o;
    int t;
    unsigned short *g = &DAT_8009c960;

    if (*g < 2 || *(int *)g == 9) {
        if (*g != 1 || !(DAT_8009d2c3 & 1)) {
            o = FUN_80018448();
            if (o) {
                o->active = 1;
                o->type = 0xc;
                o->subtype = 0;
                o->h->raw = x << 16;
                o->y.raw = y << 16;
                o->d->raw = z << 16;
                o->timer = 4;
                o->w22 = FUN_8001f9e0() & 0xf;
            }
        }
        if (f == 0 && DAT_8009c933 == 0 && DAT_8009c960 == 1 && DAT_8009c962 < 2) {
            t = DAT_8009ceaf;
            if (DAT_8009d0b4 + t >= 0x1e) return;
            t = 0x19 - DAT_8009cf0a;
            if (t > 0 && DAT_8009c959 < t && (FUN_8001f9e0() & 0xf) == 0)
                FUN_80114968(o, 0, x, y, z);
        }
    }
}
