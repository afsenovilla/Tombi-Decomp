// FUNC 800ebc30 684 I0NFO
#include "TOBJ.H"
#include "raw7.h"
extern signed char D_8009D2B0;
extern TObj D_800A6038;
extern signed char D_800A6047;

void func_800EBC30(void)
{
//PERM{
    short k;
    TObj *list[64];
    TObj *o;
    unsigned short total, by;
    short i, j, best;
    TObj **pp;
    TObj *c;
//PERM}

    for (i = 0; i < 0x40; i++) {
        list[i] = 0;
    }
    i = 0;
    total = k = *(unsigned short *)0x1F800246;
    pp = *(TObj ***)0x1F80021C;
    while (k != 0) {
        o = *pp++;
        k--;
        if ((o->type == 0x19 || (unsigned int)(o->type - 0x25) < 2) && o->_pad0e[0] == 0) {
            list[i++] = o;
        } else {
            total--;
        }
    }
    k = *(unsigned short *)0x1F80024A;
    pp = *(TObj ***)0x1F800220;
    total += k;
    while (k != 0) {
        o = *pp++;
        k--;
        if ((o->type == 0x2E || o->type == 0x1D) && o->_pad0e[0] == 0) {
            list[i++] = o;
        } else {
            total--;
        }
    }
    if (D_8009D2B0 != 3) {
        total++;
        list[i] = &D_800A6038;
    }
    for (i = 0; i < (short)total; i++) {
        best = i;
        by = list[i]->y.p.whole;
        for (j = i; j < (short)total; j++) {
            if ((short)by < list[j]->y.p.whole) {
                best = j;
                by = list[j]->y.p.whole;
            }
        }
        o = list[i];
        list[i] = list[best];
        list[best] = o;
    }
    for (i = 0; list[i] != 0; i++) {
        o = list[i];
        c = (TObj *)PTR(o, 0x94);
        o->b0f = i - 10;
        if (c != 0) {
            c->b0f = i - 9;
        }
    }
    if (D_8009D2B0 == 3) {
        D_800A6047 = -0xC;
    }
}
