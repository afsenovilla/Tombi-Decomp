// VERSION SLPM_80165 x00 800FECA8 200
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo version of playerBittenJumpReleaseFall (src/x008/playerBittenJumpReleaseFall.c): button 0x80 fixed, no fall
// lean (+0x8C set to 0x380, then cleared).
#include "TOBJ.H"
extern char *PLAYER_EXT;
extern char *BITTEN_OBJECT;
extern unsigned char GAME_BITTEN_JUMP_KIND;
extern unsigned int GAME_AREA00_EVENT_CONTROL;
extern volatile unsigned short PAD_PRESSED;

void playerBittenJumpReleaseFall(TObj *o)
{
    char *p, *q;
    int t;
    volatile unsigned short *r;
    PLAYER_EXT[8] = 0;
    *((unsigned char *)o + 0xad) = 0;
    p = PLAYER_EXT;
    q = BITTEN_OBJECT;
    o->d8c = 0x380;
    *(short *)((char *)o + 0xb2) = 0;
    *(short *)(p + 0x20) = 0xe;
    if (q[2] == 0x1c && (t = GAME_BITTEN_JUMP_KIND) < 3 && t != 0)
        *((unsigned char *)o + 0xab) |= 0x80;
    if ((GAME_AREA00_EVENT_CONTROL & 0x40) && (*(r = &PAD_PRESSED) & 0x80))
        *((unsigned char *)o + 0xa7) = 1;
    o->step = 2;
    o->d8c = 0;
    o->state = 3;
}
#endif
