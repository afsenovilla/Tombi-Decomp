// FUNC 800ee454 140 X000
#include "TOBJ.H"
extern unsigned char D_801152E8[];
static __inline__ int adj(int v, int d)
{
    if ((unsigned)d < 0x80) {
        if (d >= 4) return v + 4;
        if (d >= 2) return v + 2;
        return v + 1;
    }
    if (d < 0xfd) return v - 4;
    if (d < 0xff) return v - 2;
    return v - 1;
}
void func_800EE454(TObj *o)
{
    int d = (unsigned char)(D_801152E8[o->wb0] - o->d8c);
    if (d == 0) return;
    o->d8c = adj(o->d8c, d);
    o->d8c = *(unsigned char *)&o->d8c;
}
