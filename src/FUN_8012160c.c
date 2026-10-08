// FUNC 8012160c 272 X000
// MATCHING 8012160c 272
#include "TOBJ.H"
extern signed char DAT_80138ef4[];
extern int DAT_1f800198;
extern unsigned char DAT_1f8001f8;
extern void FUN_8012127c(TObj *);
extern TObj *FUN_80018448(void);

void FUN_8012160c(TObj *o)
{
    unsigned short u;
    TObj *n;
    unsigned int i;
    Fix16 *hp;

    u = o->timer + 1;
    o->timer = u;
    if ((u & 7) == 0)
        FUN_8012127c(o);
    o->d88 = (o->d88 + 0x18) & 0xfff;
    u = o->w22 + 1;
    o->w22 = u;
    if ((u & 7) == 0 && (n = FUN_80018448()) != 0) {
        n->active = 1;
        n->type = 0x31;
        n->subtype = 1;
        hp = n->h;
        n->b0c = (DAT_1f8001f8 + (char)DAT_1f800198) & 3;
        i = ((unsigned int)o->d88 >> 8) & 0x1fe;
        hp->raw = o->h->raw + DAT_80138ef4[i] * 0x10000;
        n->y.raw = o->y.raw + DAT_80138ef4[i + 1] * 0x10000;
        n->d->raw = o->d->raw + -0x100000;
    }
}
