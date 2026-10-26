// FUNC 800f4e9c 52 X000
// MATCHING 800f4e9c 52
#include "TOBJ.H"
extern unsigned char PLAYER_SWING_ANIM_PAIRS[][2];
typedef struct { short w0; short w2; char pad[0xa]; short we; } P;
extern P *PLAYER_EXT;

void playerSetAnimPairB(TObj *o)
{
    unsigned char *t = PLAYER_SWING_ANIM_PAIRS[o->wb2];
    PLAYER_EXT->we = t[0];
    PLAYER_EXT->w2 = t[1];
}
