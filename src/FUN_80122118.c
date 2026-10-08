// FUNC 80122118 596 X000
// MATCHING 80122118 596
#include "TOBJ.H"
#include "raw7.h"
extern char DAT_80138f04[];
extern void FUN_800202b4(TObj *);
extern void FUN_80121b28(TObj *);
extern void FUN_80121e50(TObj *);
extern void FUN_80121fe0(TObj *);
extern void FUN_80018838(TObj *);
extern void FUN_8003ecb0(void *, Fix16 *, int, int, TObj *);

void FUN_80122118(TObj *o)
{
    Fix16 v[3];

    switch (o->b04) {
    case 0:
        o->b04++;
        o->d88 = 0xc00;
        o->b0a = 0x13;
        o->w74 = 0xc00;
        o->w76 = 0xc00;
        o->w78 = 0xc00;
        o->d84 = 0;
        o->d8c = 0;
        o->b69 = 0;
        o->timer = 0x20;
        if (o->b0c == 0) {
            o->box0 = 0xe;
            o->box1 = 0x1c;
            o->box2 = 0x24;
            o->box3 = 0x24;
        } else {
            o->active = 2;
        }
        break;
    case 1:
        FUN_800202b4(o);
        if (o->b0c == 0) {
            FUN_80121b28(o);
            FUN_80121e50(o);
            o->b69 = 0;
        } else {
            o->b04 = ((TObj *)o->d90)->b04;
            o->step = ((TObj *)o->d90)->step;
            o->state = ((TObj *)o->d90)->state;
        }
        break;
    case 2:
        FUN_800202b4(o);
        switch (o->step) {
        case 0:
            if (o->b0c == 0) {
                v[0].p.whole = o->h->p.whole;
                v[1].p.whole = o->y.p.whole - 0x20;
                v[2].p.whole = o->d->p.whole;
                FUN_8003ecb0(DAT_80138f04, v, 0, -0x400, o);
                U8(PTR(o, 0xb4), 4) = 2;
                U8(PTR(o, 0xb8), 4) = 2;
                U8(PTR(o, 0xbc), 4) = 2;
                U8(PTR(o, 0xc0), 4) = 2;
                U8(PTR(o, 0xc4), 4) = 2;
                U8(PTR(o, 0xc8), 4) = 2;
            }
            o->step++;
            break;
        case 1:
            FUN_80121fe0(o);
            break;
        case 2:
            o->b04 = 3;
            break;
        }
        break;
    case 3:
        FUN_80018838(o);
        break;
    }
}
