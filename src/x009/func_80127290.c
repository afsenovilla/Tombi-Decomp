// FUNC 80127290 1084 X009
// MATCHING 80127290 1084
#include "TOBJ.H"
#include "raw7.h"

extern short D_8007A5F0[], D_8007A3F0[];
extern unsigned char D_800A6047;
extern void *D_8012EE98[];
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern short FUN_8004065c(TObj *, short, short, short);
extern short TileCollideAt(TObj *, short, short);

static __inline__ void mv(TObj *o)
{
    o->d30 = (D_8007A5F0[U8(o, 0x84)] * o->velH) >> 4;
    o->h->raw += o->d30;
    o->d84 = (o->d84 + 1) & 0xff;
    if (S16(o, 0x32) > 0) {
        if (!(o->b9d & 2) || o->animFrame != (o->b9d & 1))
            FUN_8004065c(o, o->h->p.whole - 0x10, o->y.p.whole + 0x30, 1);
    } else {
        if (!(o->b9d & 2) || o->animFrame != (o->b9d & 1))
            FUN_8004065c(o, o->h->p.whole + 0x10, o->y.p.whole + 0x30, 0);
    }
    {
        int v = D_8007A3F0[U8(o, 0x88)] * o->velV;
        o->d88 = (o->d88 + 3) & 0xff;
        o->y.raw = o->y.raw + (v >> 4) + 0x8000;
    }
    TileCollideAt(o, o->h->p.whole, o->y.p.whole + 0x30);
    AnimAdvance(o);
}

void func_80127290(TObj *o)
{
    switch (o->state) {
    case 0:
        o->state++;
        o->b0f = D_800A6047 + 1;
        if (o->animFrame)
            o->velH = -0x100;
        else
            o->velH = 0x100;
        o->velV = 0x100;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        o->wac = 0;
        o->anim = D_8012EE98[0];
        AnimLoadDuration(o);
        o->w9a = 0;
        break;
    case 1:
        mv(o);
        if (!U8(o, 0xa7)) {
            o->timer = 300;
            o->state++;
        }
        break;
    case 2:
        mv(o);
        if (--o->timer == -1) o->state++;
        if (U8(o, 0xa7)) o->state = 1;
        break;
    case 3:
        o->b04 = 1;
        o->step = 0;
        o->state = 0;
        o->active = 1;
        o->b0f -= 2;
        break;
    }
}
