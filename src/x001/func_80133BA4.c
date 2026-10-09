// FUNC 80133ba4 1228 X001
// MATCHING 80133ba4 1228
#include "TOBJ.H"

#define NEXT(o) (*(TObj **)&(o)->d94)

extern short D_8007A5F0[];
extern short D_8007A3F0[];
extern unsigned short D_1F80017E[];
extern void *D_8013F1BC[];
extern void *D_8013F200[], *D_8013F204[], *D_8013F208[], *D_8013F20C[];
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern short TileCollideAt(TObj *, short, short);

void func_80133BA4(TObj *o)
{
    TObj *n;

    switch (o->state) {
    case 0:
        o->velY = 0x10;
        o->state++;
        o->velV = 0;
        if (o->animFrame & 1) {
            o->velH = -0x400;
            o->velX = -0x20;
        } else {
            o->velH = 0x400;
            o->velX = 0x20;
        }
        o->d84 = o->d88;
        o->d->p.whole = D_1F80017E[0];
        break;
    case 1:
        o->timer = 300;
        o->w7a = 8;
        o->substep = 0;
        o->b6a = 1;
        o->state++;
        o->anim = D_8013F200[0];
        o->w74 = 0x10;
        AnimLoadDuration(o);
        for (n = NEXT(o); n; n = NEXT(n)) {
            n->b6a = 1;
            n->anim = D_8013F208[0];
            AnimLoadDuration(n);
        }
    case 2:
        AnimAdvance(o);
        for (n = NEXT(o); n; n = NEXT(n)) AnimAdvance(n);
        if (o->b69 || TileCollideAt(o, o->h->p.whole, o->y.p.whole + 0x10)) {
            o->state++;
            break;
        }
        if (o->velH) {
            o->h->raw += (D_8007A5F0[*(unsigned char *)&o->d88] * o->velH) >> 4;
            o->y.raw += (D_8007A3F0[*(unsigned char *)&o->d88] * o->velH) >> 4;
            o->velH -= o->velX;
        }
        o->y.raw += o->velV << 8;
        if (o->velV < 0x300) o->velV += o->velY;
        break;
    case 3:
        if (o->visible == 0) {
            o->timer = 0;
            o->state++;
            break;
        }
        AnimAdvance(o);
        for (n = NEXT(o); n; n = NEXT(n)) AnimAdvance(n);
        switch (o->substep) {
        case 0:
            o->w22 = 0x10;
            o->substep++;
        case 1:
            o->d84 = (o->d84 + 4) & 0xff;
            if (--o->w22 == -1) {
                o->w22 = 0x20;
                o->substep++;
            }
            break;
        case 2:
            o->d84 = (o->d84 - 4) & 0xff;
            if (--o->w22 == -1) {
                o->w22 = 0x20;
                o->substep--;
            }
            break;
        }
        if (o->timer == 0x30) {
            o->anim = D_8013F204[0];
            AnimLoadDuration(o);
            for (n = NEXT(o); n; n = NEXT(n)) {
                n->anim = D_8013F20C[0];
                AnimLoadDuration(n);
            }
        }
        if (--o->timer == -1) o->state++;
        break;
    case 4:
        o->b04 = 1;
        o->step = 0;
        o->velH = 0x100;
        if ((unsigned)(o->d88 - 0x40) < 0x80) o->state = 0x12;
        else o->state = 0x1b;
        o->w7a = 0x10;
        o->w74 = 0x20;
        o->substep = 0;
        o->active = 1;
        o->b6a = 0;
        for (n = NEXT(o); n; n = NEXT(n)) {
            n->active = 1;
            n->b6a = 0;
            n->anim = D_8013F1BC[0];
        }
        break;
    }
    if ((unsigned)(o->d88 - 0x40) < 0x80) o->animFrame = 1;
    else o->animFrame = 0;
}
