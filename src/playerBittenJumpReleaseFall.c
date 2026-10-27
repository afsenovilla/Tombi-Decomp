// FUNC 80105ae4 232 X000
// MATCHING 80105ae4 232
#include "TOBJ.H"
extern char *PLAYER_EXT;
extern char *BITTEN_OBJECT;
extern unsigned char GAME_BITTEN_JUMP_KIND;
extern unsigned int GAME_AREA00_EVENT_CONTROL;
extern volatile unsigned short PAD_PRESSED;
extern unsigned short DAT_1f8003c4;
extern void playerSetFallLean(TObj *o);

void playerBittenJumpReleaseFall(TObj *o)
{
    char *p, *q;
    int t;
    volatile unsigned short *r;
    PLAYER_EXT[8] = 0;
    *((unsigned char *)o + 0xad) = 0;
    p = PLAYER_EXT;
    q = BITTEN_OBJECT;
    *(short *)((char *)o + 0xb2) = 0;
    *(short *)(p + 0x20) = 0xe;
    if (q[2] == 0x1c && (t = GAME_BITTEN_JUMP_KIND) < 3 && t != 0)
        *((unsigned char *)o + 0xab) |= 0x80;
    if ((GAME_AREA00_EVENT_CONTROL & 0x40) && (*(r = &PAD_PRESSED) & DAT_1f8003c4))
        *((unsigned char *)o + 0xa7) = 1;
    playerSetFallLean(o);
    o->step = 2;
    o->state = 3;
}
