// FUNC 800ee454 140 X000
#include "TOBJ.H"
extern unsigned char D_801152E8[];
void func_800EE454(TObj *o)
{
    unsigned char d = D_801152E8[o->wb0] - o->d8c;
    unsigned int u;
    int v;
    if (d == 0) return;
    u = d;
    if (u < 0x80) {
        if (d >= 4) v = o->d8c + 4;
        else if (d >= 2) v = o->d8c + 2;
        else v = o->d8c + 1;
    } else {
        if (d < 0xfd) v = o->d8c - 4;
        else if (d < 0xff) v = o->d8c - 2;
        else v = o->d8c - 1;
    }
    o->d8c = v;
    o->d8c = *(unsigned char *)&o->d8c;
}
