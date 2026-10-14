// FUNC 80028b84 92 MAIN0
#include "spad.h"
typedef struct {
    char pad[0x30];
    short x30;
    short x32;
} O80028B84;


static __inline__ void clamp(O80028B84 *o, short *g)
{
    short x = g[0];
    int s = x + g[-6];
    int v;

    if (s < (v = o->x30) || (v = o->x32) < s) {
        g[-6] = v - x;
    }
}

void func_80028B84(O80028B84 *o)
{
    clamp(o, ((short*)((char*)&SPAD.if0 + 2)));
}
