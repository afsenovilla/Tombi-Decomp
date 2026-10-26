// FUNC 800f4e68 52 X009
// MATCHING 800f4e68 52
#include "TOBJ.H"
typedef struct { char p0[2]; short a; char p1[0xa]; short b; } G;
extern G *PLAYER_EXT;
extern unsigned char PLAYER_PERCH_ANIM_PAIRS[];

void playerSetAnimPairA(TObj *o)
{
    unsigned char *p = &PLAYER_PERCH_ANIM_PAIRS[o->wb2 * 2];
    G *g = PLAYER_EXT;
    g->b = p[0];
    g->a = p[1];
}
