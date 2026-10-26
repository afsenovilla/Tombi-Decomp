// FUNC 800f4e68 52 X005
// MATCHING 800f4e68 52
#include "TOBJ.H"
typedef struct { char p0[2]; short a; char p1[0xa]; short b; } G;
extern G *PLAYER_EXT;
extern unsigned char D_80115220[];

void playerSetAnimPairA(TObj *o)
{
    unsigned char *p = &D_80115220[o->wb2 * 2];
    G *g = PLAYER_EXT;
    g->b = p[0];
    g->a = p[1];
}
