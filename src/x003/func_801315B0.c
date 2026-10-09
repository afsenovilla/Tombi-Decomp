// FUNC 801315b0 820 X003
// MATCHING 801315b0 820
#include "TOBJ.H"
typedef struct { unsigned short x, y; } XY;
extern TObj D_800A6038;
extern XY D_80135F10;
extern XY D_80135F14[];
extern XY D_80135F24[];
extern unsigned short D_80135F34[];
extern int FUN_8001f9e0(void);
extern TObj *FUN_800183b8(void);
extern void FUN_80018980(TObj *);

#define W1C(o) (*(unsigned short *)((char *)(o) + 0x1c))

void func_801315B0(TObj *o)
{
    TObj *pl = &D_800A6038;
    TObj *e;
    unsigned short x, y;

    switch (o->b04) {
    case 0:
        W1C(o) = 0;
        o->b04++;
        o->w1e = 0;
        o->timer = 0;
        o->step = o->subtype * 2;
        break;
    case 1:
        switch (o->step) {
        case 0:
            o->w08 = (FUN_8001f9e0() & 7) * 8 + 0x14;
            o->step++;
            break;
        case 1:
            if (W1C(o) >= 2) break;
            if (pl->b.p.whole != 0) break;
            if ((unsigned short)(pl->a.p.whole - 0x3e5) >= 0x98) break;
            if (--o->w08 != -1) break;
            x = D_80135F10.x;
            y = D_80135F10.y;
            goto spawn;
        case 2:
            o->w08 = (FUN_8001f9e0() & 7) * 8 + 1;
            o->step++;
            break;
        case 3:
            if (W1C(o) >= 2) break;
            if (pl->b.p.whole != 0) break;
            if ((unsigned short)(pl->a.p.whole - 0x834) >= 0x119) break;
            if (--o->w08 != -1) break;
            {
                XY *t;
                x = FUN_8001f9e0() & 3;
                t = &D_80135F14[x];
                x = t->x;
                y = t->y;
            }
            goto spawn;
        case 4:
            o->w08 = (FUN_8001f9e0() & 7) * 8 + 10;
            o->step++;
            break;
        case 5:
            if (W1C(o) >= 2) break;
            if (pl->b.p.whole != 0) break;
            if ((unsigned short)(pl->a.p.whole - 0x921) >= 0xbd) break;
            if (--o->w08 != -1) break;
            {
                XY *t;
                x = FUN_8001f9e0() & 3;
                t = &D_80135F24[x];
                x = t->x;
                y = t->y;
            }
        spawn:
            e = FUN_800183b8();
            if (e) {
                e->active = 2;
                e->type = 2;
                e->b0a = 2;
                e->subtype = 2;
                e->b0c = 4;
                e->b.p.whole = 0x3c;
                e->animFrame = 0;
                e->a.p.whole = x;
                e->y.p.whole = y;
                e->d90 = (int)&W1C(o);
                W1C(o)++;
            }
            o->timer = (o->timer + 8) & 8;
            o->w08 = D_80135F34[(FUN_8001f9e0() & 7) + (o->timer & 0xffff)];
            break;
        }
        break;
    case 2:
    case 3:
        FUN_80018980(o);
        break;
    }
}
