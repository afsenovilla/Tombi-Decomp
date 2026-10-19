// FUNC 800ed4d0 796 I0NFO2
#include "TOBJ.H"
#include "raw7.h"
extern signed char D_8009D2B0;
extern TObj D_800A6038;
extern signed char D_800A6047;
#define LI(n) (*(TObj **)((char *)list + ((n) << 2)))
#define PI_TOTAL asm("t0")
#define PI_I 
#define PI_J
#define PI_S 
#define PI_BY asm("a1")
#define PI_C asm("a1")
#define PI_K1 asm("a1")
#define PI_PP1 
#define PI_O1 
#define PI_K2 asm("a1")
#define PI_PP2 
#define PI_O2 
#define PI_O3 
#define PI_O4 

void func_800ED4D0(void)
{
    TObj *list[64];
    register unsigned short total PI_TOTAL;
    register short i PI_I;
    register short j PI_J;
    short best;
    register short s PI_S;
    register unsigned short by PI_BY;
    register TObj *c PI_C;

    for (i = 0; i < 0x40; i++) {
        *(TObj **)((char *)list + (i << 2)) = 0;
    }
    i = 0;
    {
    register short k PI_K1;
    register TObj **pp PI_PP1;
    register TObj *o PI_O1;
    k = total = *(unsigned short *)0x1F800246;
    pp = *(TObj ***)0x1F80021C;
    if (total != 0) do {
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
    } while (k != 0);
    }
    {
    register short k PI_K2;
    register TObj **pp PI_PP2;
    register TObj *o PI_O2;
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
    }
    if (D_8009D2B0 != 3) {
        total++;
        LI(i) = &D_800A6038;
    }
    {
    unsigned short cnt = total;
    register TObj *o PI_O3;
    for (s = 0; s < (short)cnt; s++) {
        best = s;
        by = LI(s)->y.p.whole;
        for (j = s; j < (short)cnt; j++) {
            if ((short)by < LI(j)->y.p.whole) {
                best = j;
                by = LI(j)->y.p.whole;
            }
        }
        o = list[s];
        list[s] = list[best];
        list[best] = o;
    }
    }
    {
    register TObj *o PI_O4;
    for (i = 0; LI(i) != 0; i++) {
        o = LI(i);
        c = (TObj *)PTR(o, 0x94);
        S8(o, 0xF) = i - 10;
        if (c != 0) {
            S8(c, 0xF) = i - 9;
        }
    }
    }
    if (D_8009D2B0 == 3) {
        D_800A6047 = -0xC;
    }
}
