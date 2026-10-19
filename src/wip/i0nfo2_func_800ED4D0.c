// FUNC 800ed4d0 796 I0NFO2
#include "TOBJ.H"
#include "raw7.h"
extern signed char D_8009D2B0;
extern TObj D_800A6038;
extern signed char D_800A6047;
#define LI(n) (*(TObj **)((char *)list + ((n) << 2)))

void func_800ED4D0(void)
{
    short k;
    TObj *list[64];
    TObj *o;
    unsigned short total, by;
    short i, j, best, s;
    TObj **pp;
    TObj *c;

    for (i = 0; i < 0x40; i++) {
        *(TObj **)((char *)list + (i << 2)) = 0;
    }
    i = 0;
    total = k = *(unsigned short *)0x1F800246;
    pp = *(TObj ***)0x1F80021C;
    while (k != 0) {
        o = *pp++;
        k--;
        if (o->type != 0x30 && (unsigned int)(o->type - 0x2D) >= 2) {
        fail1:
            total--;
        } else if (o->_pad0e[0] != 0) {
            switch (o->_pad0e[0]) {
            default:
                goto fail1;
            case 1:
                S8(o, 0xF) = -0xC;
                goto fail1;
            case 2:
                S8(o, 0xF) = 0xC;
                goto fail1;
            }
        } else {
            LI(i++) = o;
        }
    }
    k = *(unsigned short *)0x1F80024A;
    pp = *(TObj ***)0x1F800220;
    total += k;
    while (k != 0) {
        o = *pp++;
        k--;
        if (o->type != 0x1D) {
        fail2:
            total--;
            continue;
        }
        switch (o->_pad0e[0]) {
        default:
            goto fail2;
        case 1:
            S8(o, 0xF) = -0xC;
            goto fail2;
        case 2:
            S8(o, 0xF) = 0xC;
            goto fail2;
        case 0:
            LI(i++) = o;
            break;
        }
    }
    if (D_8009D2B0 != 3) {
        total++;
        LI(i) = &D_800A6038;
    }
    for (s = 0; s < (short)total; s++) {
        best = s;
        by = LI(s)->y.p.whole;
        for (j = s; j < (short)total; j++) {
            if ((short)by < LI(j)->y.p.whole) {
                best = j;
                by = LI(j)->y.p.whole;
            }
        }
        o = list[s];
        list[s] = list[best];
        list[best] = o;
    }
    for (i = 0; LI(i) != 0; i++) {
        o = LI(i);
        c = (TObj *)PTR(o, 0x94);
        S8(o, 0xF) = i - 10;
        if (c != 0) {
            S8(c, 0xF) = i - 9;
        }
    }
    if (D_8009D2B0 == 3) {
        D_800A6047 = -0xC;
    }
}
