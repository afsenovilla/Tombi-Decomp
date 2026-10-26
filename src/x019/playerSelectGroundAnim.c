// FUNC 800efa80 388 X019
// MATCHING 800efa80 388
#include "TOBJ.H"
typedef struct { char pad[0x2c]; unsigned short cur; unsigned short prev; } Cam2c;
extern Cam2c *PLAYER_EXT;
extern int DAT_8009c984;
extern unsigned short PAD_PRESSED;
extern unsigned short DAT_1f8003c4;
extern void playerGroundAnimTransition(TObj *o, int v);

void playerSelectGroundAnim(TObj *o)
{
    short v;
    if (o->wb2 == 0) {
        PLAYER_EXT->cur = 0;
    } else {
        if ((DAT_8009c984 & 0x40) && (*(volatile unsigned short *)&PAD_PRESSED & DAT_1f8003c4) && o->b04 == 1) {
            PLAYER_EXT->cur = 0x21;
        } else {
            v = o->wb2;
            if (v < -0x143)
                PLAYER_EXT->cur = 3;
            else if (v < -0x84)
                PLAYER_EXT->cur = 2;
            else if (v < 0)
                PLAYER_EXT->cur = 1;
            else if (v >= 0x144)
                PLAYER_EXT->cur = 3;
            else if (v >= 0x85)
                PLAYER_EXT->cur = 2;
            else if (v > 0)
                PLAYER_EXT->cur = 1;
        }
        if (o->subtype)
            PLAYER_EXT->cur = 0x21;
    }
    if (PLAYER_EXT->prev != PLAYER_EXT->cur) {
        playerGroundAnimTransition(o, PLAYER_EXT->cur);
        PLAYER_EXT->prev = PLAYER_EXT->cur;
    }
}
