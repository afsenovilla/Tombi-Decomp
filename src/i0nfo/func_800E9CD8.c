// FUNC 800e9cd8 648 I0NFO
// MATCHING 800e9cd8 648
#include "TOBJ.H"
#include "raw7.h"
extern void func_8001FC14(TObj *o, int a, int b);
extern void func_8001FCE4(TObj *o);
extern void func_8001FEC0(TObj *o);
extern void func_800E88A0(TObj *o);
extern void func_800E8D60(TObj *o);
extern void func_800E947C(TObj *o);
extern void func_800E962C(TObj *o);
extern void func_800EC88C(TObj *o, int x, int y, int z);
extern TObj *D_8009C330;
extern unsigned char D_8009D00F;

void func_800E9CD8(TObj *o)
{
    unsigned short k;
    switch (o->step) {
    case 0:
        if (D_8009D00F != 0) {
            func_800E88A0(o);
        } else if (o->active == 5) {
            func_8001FEC0(o);
        } else {
            D_8009C330->animTimer = o->animFrame |= 8;
            func_800E8D60(o);
        }
        break;
    case 1:
        switch (o->state) {
        case 0:
            D_8009C330->animTimer = o->animFrame |= 8;
            func_800E8D60(o);
            o->b9e = 1;
            o->state++;
        case 1:
            func_8001FEC0(o);
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
            D_8009C330->animTimer = o->animFrame &= 7;
            func_800E8D60(o);
            o->b9e = 1;
            o->state++;
        case 1:
            func_8001FEC0(o);
            k = o->animFrame & 7;
            o->wb6 = k << 5;
            func_8001FC14(o, k << 5, o->wb2);
            func_8001FCE4(o);
            func_800EC88C(o, o->h->p.whole, o->y.p.whole, (short)o->animFrame);
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
            D_8009C330->animTimer = o->animFrame |= 8;
            func_800E8D60(o);
            o->state++;
        case 1:
            if (D_8009D00F != 0) func_800E88A0(o);
            else func_800E8D60(o);
            break;
        }
        break;
    case 4:
        func_800E947C(o);
        break;
    case 5:
        func_800E962C(o);
        break;
    case 6:
        o->visible = 1;
        break;
    case 7:
        break;
    }
}
