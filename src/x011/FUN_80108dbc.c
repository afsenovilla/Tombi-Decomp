// FUNC 80108dbc 420 X011
// MATCHING 80108dbc 420
#include "TOBJ.H"
#include "raw7.h"
extern TObj *DAT_8009c330;
extern unsigned char DAT_8009c93a;
extern unsigned short D_1f8001c8;
extern unsigned char DAT_801152e8[];
extern void PlayerSetAnimIfChanged(TObj *, int);
extern void FUN_8001fec0(TObj *);

void FUN_80108dbc(TObj *o)
{
    switch (o->state) {
    case 0:
        U8(DAT_8009c330, 8) = o->active;
        U8(DAT_8009c330, 9) = 0;
        o->velX = 0x5a;
        o->active = 3;
        U8(o, 0xa3) = 2;
        o->d8c = 0;
        o->velY = 0;
        o->b9c = 0;
        o->b9d = 0;
        o->b9e = 0;
        o->b9f = 0;
        U8(o, 0xad) = 0;
        o->b69 = 0;
        o->state++;
        o->animFrame &= 1;
        PlayerSetAnimIfChanged(o, 0x2c);
    case 1:
        FUN_8001fec0(o);
        if (D_1f8001c8 & 1)
            o->d->p.whole += 2;
        else
            o->d->p.whole -= 2;
        if ((o->velX -= 2) == 0)
            o->state = 9;
        break;
    case 9:
        DAT_8009c93a = 1;
        S8(o, 0xf) = -8;
        o->ba5 = 0;
        o->b9c = 0;
        U8(o, 0xac) = 0;
        o->wb2 = 0;
        o->velX = 0;
        o->velY = 0;
        DAT_8009c330->timer = 0;
        o->active = 1;
        o->d8c = DAT_801152e8[o->wb0];
        o->b04 = 1;
        o->step = 0;
        o->state = 0;
        break;
    }
}
