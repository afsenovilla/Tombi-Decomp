// FUNC 80121f90 1160 X014
// MATCHING 80121f90 1160
#include "TOBJ.H"
typedef struct { short s0, s2; } S2;
extern TObj D_800A6038;
extern S2 *D_800A6078;
extern unsigned char D_800A603C, D_800A603D, D_800A603E, D_800A60D6, D_8009C93A;
extern short D_800A6066;
extern short D_800A457C, D_800A457E;
extern unsigned short D_1F80027E;
extern short func_80040278(TObj *, short, short);

void func_80121F90(TObj *o)
{
    TObj *pl;
    switch (o->step) {
    case 0:
        o->velH = 0x80;
        o->timer = 0x1e;
        o->step++;
        break;
    case 1:
    case 3:
    case 6:
        if (--o->timer == -1) o->step++;
        break;
    case 2:
        if (D_8009C93A != 0) {
            o->step++;
            if (o->subtype == 3) o->timer = 10;
            else o->timer = 0x1fe;
        }
        break;
    case 4:
        o->a.raw -= o->velH << 8;
        if (o->h->p.whole < D_800A457C - 0x96) {
            if (o->subtype == 3) {
                pl = &D_800A6038;
                if (D_800A60D6 == 3) {
                    pl->active = 2;
                    D_800A603C = 2;
                    D_800A603D = 0;
                    D_800A603E = 0;
                    D_800A6066 = o->h->p.whole > D_800A6078->s2;
                }
            }
            o->step++;
        } else {
            if (o->h->p.whole < D_800A457E + 0xa0) {
                short y = o->y.p.whole + 4;
                o->y.p.whole = y;
                if (func_80040278(o, o->h->p.whole, (short)(y + o->box2))) {
                    o->wb2 = D_1F80027E;
                    goto d4;
                }
            }
            o->wb2 = 0;
        }
    d4:
        if (o->wb2 != 0) {
            if (o->wb2 < 0) {
                o->velH += 0x20;
                if (o->velH > 0x200) o->velH = 0x200;
            } else {
                o->velH -= 0x10;
                if (o->velH < 0x80) o->velH = 0x80;
            }
        }
        o->d8c -= o->velH >> 5;
        break;
    case 5:
        o->a.raw -= o->velH << 8;
        o->d8c -= o->velH >> 5;
        if (o->a.p.whole < 0x22) {
            o->timer = 0x14;
            o->step++;
        }
        break;
    case 7:
        o->a.raw += o->velH << 8;
        if (D_800A457E + 0x96 < o->h->p.whole) {
            if (o->subtype == 3) {
                pl = &D_800A6038;
                if (D_800A60D6 == 3) {
                    pl->active = 2;
                    D_800A603C = 2;
                    D_800A603D = 0;
                    D_800A603E = 0;
                    D_800A6066 = o->h->p.whole > D_800A6078->s2;
                }
            }
            o->step++;
        } else {
            if (D_800A457C - 0xa0 < o->h->p.whole) {
                short y = o->y.p.whole + 4;
                o->y.p.whole = y;
                if (func_80040278(o, o->h->p.whole, (short)(y + o->box2))) {
                    o->wb2 = D_1F80027E;
                    goto d7;
                }
            }
            o->wb2 = 0;
        }
    d7:
        o->d8c += o->velH >> 5;
        if (o->wb2 != 0) {
            if (o->wb2 > 0) {
                o->velH += 0x20;
                if (o->velH > 0x200) o->velH = 0x200;
            } else {
                o->velH -= 0x10;
                if (o->velH < 0x80) o->velH = 0x80;
            }
        }
        break;
    case 8:
        o->a.raw += o->velH << 8;
        o->d8c += o->velH >> 5;
        if (o->a.p.whole >= 0x39f) {
            o->step = 3;
            o->timer = 0x14;
        }
        break;
    }
}
