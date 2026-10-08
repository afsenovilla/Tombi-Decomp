// FUNC 80118390 316 X014
// MATCHING 80118390 316
#include "TOBJ.H"
extern int FUN_8001f9e0(void);
extern int FUN_800202b4(TObj *);

void func_80118390(TObj *o)
{
    switch (o->step) {
    case 0:
        o->step++;
        o->timer = 0x10;
        o->animFrame = (FUN_8001f9e0() & 0x7f) + 0x40;
        o->wb6 = 0x30;
        o->wb4 = -2;
        o->wbc = 2;
        *(short *)&o->bbe = 0x30;
        o->d84 = 0;
        o->d88 = 0;
        o->wb8 = 0;
        *(short *)((char *)o + 0xc0) = 0;
        *(short *)((char *)o + 0xc4) = 0;
        *(short *)((char *)o + 0xc6) = 0;
        *(short *)((char *)o + 0xc8) = 0;
        o->w74 = 0;
        o->w76 = 0;
        o->w78 = 0xf0;
        o->d8c = (o->animFrame & 0xff) << 4;
        o->wac = FUN_8001f9e0() & 3;
        if (o->wac == 3) o->wac = 0;
        break;
    case 1:
        if ((o->timer & 3) == 0) FUN_800202b4(o);
        if (--o->timer == -1) o->b04 = 3;
        o->wb6 += 2;
        *(short *)&o->bbe += 2;
        *(short *)((char *)o + 0xc6) += 2;
        break;
    }
}
