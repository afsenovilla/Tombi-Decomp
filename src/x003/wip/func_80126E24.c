// FUNC 80126e24 276 X003
/* score 23: game loads o->state (lbu) first in case 0, before the movetab store, and has state/table in v1/v0 swapped; tried all orders of the 5 case-0 statements, scalar vs [0] anim, state++/+=1/local copy. Sibling func_80126D18 matched. */
#include "TOBJ.H"

extern unsigned char D_80135CB0[];
extern void *D_80139578[];
extern char D_80077CDC[];
extern short D_1F80027E;
extern void AnimLoadDuration(TObj *);
extern void FUN_8001fb20(TObj *);
extern short TileCollideAt(TObj *, short, short);

void func_80126E24(TObj *o)
{
    unsigned char *p;

    switch (o->state) {
    case 0:
        o->wac = 0x1e;
        o->state++;
        o->anim = D_80139578[0];
        o->movetab = D_80077CDC;
        p = D_80135CB0 + (o->wac << 2);
        o->box0 = *p++;
        o->box1 = *p++;
        o->box2 = *p;
        o->box3 = p[1];
        AnimLoadDuration(o);
    case 1:
        FUN_8001fb20(o);
        if (TileCollideAt(o, o->h->p.whole, o->y.p.whole + 0xe) != 0) {
            o->d8c = (-D_1F80027E * 4) & 0xff;
            o->state = 0;
            o->step++;
        }
        break;
    }
}
