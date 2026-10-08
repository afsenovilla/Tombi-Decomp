// FUNC 800f4e9c 52 X000
// MATCHING 800f4e9c 52
#include "TOBJ.H"
extern unsigned char D_80115228[][2];
typedef struct { short w0; short w2; char pad[0xa]; short we; } P;
extern P *D_8009C330;

void func_800F4E9C(TObj *o)
{
    unsigned char *t = D_80115228[o->wb2];
    D_8009C330->we = t[0];
    D_8009C330->w2 = t[1];
}
