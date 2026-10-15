// JP slps01144 800551D4 936
// Japanese version of PAL main0 8004A124 (eventCarryPlayerB.c): 242 -> 226.
#include "gcd.h"
#include "TOBJ.H"
#include "raw7.h"

extern TObj PLAYER;
extern void playerSetAnim(TObj *, int, int);
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern short TileCollideAt(TObj *, short, short);

void eventCarryPlayerB(TObj *o)
{
    TObj *s = &PLAYER;

    switch (o->state) {
    case 0:
        if (((*(int*)&s->b04) & 0xffffff) != 0x30405)
            break;
        o->h->p.whole = s->h->p.whole;
        s->h->p.whole = o->h->p.whole;
        s->y.p.whole = o->y.p.whole - 0x14;
        o->b0f = s->b0f - 2;
        s->active = 5;
        s->visible = 1;
        s->b04 = 5;
        s->step = 0x41;
        s->state = 0;
        playerSetAnim(s, 0x2c, 0);
        o->wac = 1;
        o->anim = ((void **)(*(int*)&o->wa8))[1];
        AnimLoadDuration(o);
        o->velV = 0x500;
        o->state++;
        break;
    case 1:
        o->velV -= 8;
        if (o->velV < 0x100)
            o->velV = 0x100;
        o->y.raw += o->velV << 8;
        s->y.raw += o->velV << 8;
        if (o->y.p.whole < S16(s, 0xe2) - 0x1e)
            break;
        if (TileCollideAt(o, o->h->p.whole, o->y.p.whole + 0x10)) {
            o->timer = 0x1e;
            o->wac = 0;
            o->state++;
            o->anim = ((void **)(*(int*)&o->wa8))[0];
            AnimLoadDuration(o);
        }
        break;
    case 2:
        if (--o->timer == -1)
            o->state++;
        break;
    case 3:
        s->y.raw += 0x8000;
        if (TileCollideAt(s, s->h->p.whole, s->y.p.whole + 0x14)) {
            o->wac = 0;
            o->state++;
            o->anim = ((void **)(*(int*)&o->wa8))[0];
            AnimLoadDuration(o);
        }
        break;
    case 4:
        o->timer = 0x10;
        s->animFrame = 0;
        playerSetAnim(s, 1, 0);
        o->state++;
        break;
    case 5:
        AnimAdvance(s);
        s->h->p.whole++;
        if (--o->timer == -1) {
            o->timer = 0x1e;
            o->state++;
            s->animFrame = 0;
            playerSetAnim(s, 0, 0);
        }
        break;
    case 6:
        if (--o->timer == -1) {
            o->state++;
            o->b0f = s->b0f + 1;
            o->wac = 3;
            o->animFrame = 0;
            o->anim = ((void **)(*(int*)&o->wa8))[3];
            AnimLoadDuration(o);
        }
        break;
    case 7:
        AnimAdvance(o);
        o->h->p.whole++;
        if (o->h->p.whole >= s->h->p.whole) {
            s->b04 = 5;
            s->step = 4;
            s->state = 2;
            GCD.bcff9 = 0;
            o->active = 2;
            o->b04 = 3;
        }
        break;
    }
}
