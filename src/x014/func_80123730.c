// FUNC 80123730 892 X014
// MATCHING 80123730 892
/* whole function: notes/functions_x014.csv splits it at 801237e4 (func_801237E4 is its tail) */
#include "TOBJ.H"
extern TObj *FUN_800183b8(void);

#define SPAWN(C, K, V)                    \
    e = FUN_800183b8();                   \
    if (e != 0) {                         \
        e->active = 1;                    \
        e->type = 0x4b;                   \
        e->b0f = o->b0f + 1;              \
        e->subtype = 2;                   \
        e->b0c = (C);                     \
        e->b0a = 0;                       \
        e->b6b = (K);                     \
        e->velH = (V);                    \
        e->a.p.whole = o->a.p.whole;      \
        e->y.p.whole = o->y.p.whole;      \
        e->b.p.whole = o->b.p.whole;      \
    }

void func_80123730(TObj *o)
{
    TObj *e;
    int i, j, k, l, m, n;
    register short v asm("$19"); /* debt: plain short v gets s2 (the hoisted constant 1 should) */

    v = 0xb0;
    if (o->a.p.whole < 0xa0) {
        for (i = 0; i < 3; i++) { SPAWN(i + 2, i, v) }
        v = 0xb0;
        for (j = 0; j < 3; j++) { SPAWN(j + 5, j + 3, v) }
    } else if (o->a.p.whole >= 0x21d) {
        v = -0xb0;
        for (k = 0; k < 3; k++) { SPAWN(k + 2, k, v) }
        v = -0xb0;
        for (l = 0; l < 3; l++) { SPAWN(l + 5, l + 3, v) }
    } else {
        for (m = 0; m < 3; m++) { SPAWN(m + 2, m, v) }
        v = -0xb0;
        for (n = 0; n < 3; n++) { SPAWN(n + 5, n, v) }
    }
}
