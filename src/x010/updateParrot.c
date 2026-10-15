// FUNC 8012c8a0 688 X010
// MATCHING 8012c8a0 688
#include "spad.h"
#include "gcd.h"
#include "TOBJ.H"
typedef struct { short x, y, w, h; } RECT;

extern void *D_801322E8[], *D_801322F4[];
extern short D_8007A3F0[];
extern void MoveImage(RECT *, int, int);
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern int ObjCullRegister(TObj *);
extern void freeObjectLayer2(TObj *);
extern void parrotPumpRocksClutFlash(TObj *);
extern void nobleTreeBiteTalk(TObj *);
extern void nobleTreeAccordionRocksHint(TObj *);
extern void nobleTreeRestoredTalk(TObj *);
extern void nobleTreeGrandpaMessage(TObj *);
extern void nobleTreeGoldenItemsAsk(TObj *);
extern void nobleTreeGoldenItemsHint(TObj *);
extern void func_8012C6EC(TObj *);

void updateParrot(TObj *o)
{
    RECT r;

    switch (o->b04) {
    case 0:
        o->b6b = 0;
        if (GCD.bcde3 == 0xff) {
            r.x = 0x120;
            r.y = 0x1e4;
            r.w = 0x10;
            r.h = 4;
            MoveImage(&r, 0x120, 0x1e0);
            o->b6b = 5;
        }
        o->box0 = 4;
        o->box1 = 8;
        o->box2 = 6;
        o->box3 = 0xc;
        *(signed char *)&o->b0f = -6;
        o->w1e = 3;
        o->animFrame = 0;
        o->b0d = 0;
        o->wac = 2;
        o->b04++;
        o->anim = D_801322E8[0];
        o->d3c = ((int*)&SPAD.i2d0)[0];
        AnimLoadDuration(o);
        break;
    case 1:
        parrotPumpRocksClutFlash(o);
        ObjCullRegister(o);
        AnimAdvance(o);
        switch (o->step) {
        case 0:
            switch (o->state) {
            case 0:
                o->active = 5;
                o->velV = 0x100;
                o->d88 = 0;
                o->wac = 5;
                o->anim = D_801322F4[0];
                AnimLoadDuration(o);
                o->state++;
                break;
            case 1:
                AnimAdvance(o);
                o->y.raw += (D_8007A3F0[o->d88] * o->velV) >> 4;
                o->d88 = (o->d88 + 2) & 0xff;
                break;
            }
            break;
        case 1:
            nobleTreeBiteTalk(o);
            break;
        case 2:
            nobleTreeAccordionRocksHint(o);
            break;
        case 3:
            nobleTreeRestoredTalk(o);
            break;
        case 4:
            nobleTreeGrandpaMessage(o);
            break;
        case 5:
            nobleTreeGoldenItemsAsk(o);
            break;
        case 6:
            nobleTreeGoldenItemsHint(o);
            break;
        }
        break;
    case 2:
        ObjCullRegister(o);
        func_8012C6EC(o);
        break;
    case 3:
        freeObjectLayer2(o);
        break;
    }
}
