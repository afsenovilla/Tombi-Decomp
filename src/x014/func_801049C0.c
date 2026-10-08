// FUNC 801049c0 68 X014
// MATCHING 801049c0 68
#include "TOBJ.H"
typedef struct { char p[8]; unsigned char b; } G;
extern G *D_8009C330;
extern unsigned char D_801152E8[];
void func_801049C0(TObj *o)
{
    D_8009C330->b = 0;
    o->b9c = 0;
    o->ba7 = 0;
    o->wb2 = 0;
    o->velY = 0;
    *(unsigned char *)&o->wac = 0;
    o->d8c = D_801152E8[o->wb0];
    o->step = 0;
    o->state = 0;
}
