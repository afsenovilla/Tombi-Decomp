// FUNC 8003511c 124 MAIN0
// MATCHING 8003511c 124
#include "player.h"
typedef struct P { short x; short y; } P;
typedef struct S { char p[0x16]; short z; char q[0x28]; P *pos; } S;
extern P *D_800A6078;
extern int csqrt(int);
extern int abs(int);
short func_8003511C(S *o)
{
    int dx = abs(o->pos->y - D_800A6078->y);
    int dz = abs(o->z - PLAYER.obj.y.p.whole);
    return csqrt((dx * dx + dz * dz) << 12) >> 12;
}
