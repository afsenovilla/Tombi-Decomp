// FUNC 80126e24 276 X003
// MATCHING 80126e24 276
#include "TOBJ.H"

extern unsigned char D_80135CB0[];
extern void *D_80139578;
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
        o->movetab = D_80077CDC;
        o->wac = 0x1e;
        o->state++;
        o->anim = D_80139578;
        p = &D_80135CB0[o->wac * 4];
        o->box0 = *p++;
        o->box1 = *p++;
        o->box2 = *p++;
        o->box3 = *p++;
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
