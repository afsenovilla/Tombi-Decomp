// FUNC 800f4e9c 52 X005
// MATCHING 800f4e9c 52
#include "TOBJ.H"
typedef struct { char p0[2]; short a; char p1[0xa]; short b; } G;
extern G *D_8009C330;
extern unsigned char D_80115228[];

void func_800F4E9C(TObj *o)
{
    unsigned char *p = &D_80115228[o->wb2 * 2];
    G *g = D_8009C330;
    g->b = p[0];
    g->a = p[1];
}
