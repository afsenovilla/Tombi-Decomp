// VERSION SLPS_01144 info2 800ECAA8 508
// JP version of PAL i0nfo2 800eb2c8 (topdownPlayerEventStep.c)
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"
#include "raw7.h"
extern void func_80020B8C(TObj *o, int a, int b);
extern void func_80020C5C(TObj *o);
extern void func_80020E38(TObj *o);
extern void overheadPlayerSetMoveAnim(TObj *o);
extern void func_800EC204(TObj *o);
extern void func_800EC368(TObj *o);
extern void overheadCollideMapTile(TObj *o, int x, int y, int z);
extern TObj *D_8009DEE0;

void func_800ECAA8(TObj *o)
{
    unsigned short k;
    switch (o->step) {
    case 0:
        func_80020E38(o);
        break;
    case 1:
        switch (o->state) {
        case 0:
            D_8009DEE0->animTimer = o->animFrame |= 8;
            overheadPlayerSetMoveAnim(o);
            o->b9e = 1;
            o->state++;
        case 1:
            func_80020E38(o);
            if (--o->timer <= 0) {
                o->b9e = 0;
                o->step = 0;
                o->state = 0;
            }
            break;
        }
        break;
    case 2:
        U8(o, 0xAC) = 0;
        switch (o->state) {
        case 0:
            D_8009DEE0->animTimer = o->animFrame &= 7;
            overheadPlayerSetMoveAnim(o);
            o->b9e = 1;
            o->state++;
        case 1:
            func_80020E38(o);
            k = o->animFrame & 7;
            o->wb6 = k << 5;
            func_80020B8C(o, k << 5, o->wb2);
            func_80020C5C(o);
            overheadCollideMapTile(o, o->h->p.whole, o->y.p.whole, (short)o->animFrame);
            if (--o->timer <= 0) {
                o->b9e = 0;
                o->step = 0;
                o->state = 0;
            }
            break;
        }
        break;
    case 3:
        U8(o, 0xAC) = 0;
        switch (o->state) {
        case 0:
            D_8009DEE0->animTimer = o->animFrame |= 8;
            overheadPlayerSetMoveAnim(o);
            o->state++;
        }
        break;
    case 4:
        func_800EC204(o);
        break;
    case 5:
        func_800EC368(o);
        break;
    case 6:
        o->visible = 1;
        break;
    case 7:
        break;
    }
}
#endif
