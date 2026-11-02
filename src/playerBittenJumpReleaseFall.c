// FUNC 80105ae4 232 X000
// MATCHING 80105ae4 232
// VERSION SLPS_01144 x00 801057BC 236
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"
extern char *PLAYER_EXT;
extern char *BITTEN_OBJECT;
extern unsigned char GAME_BITTEN_JUMP_KIND;
extern unsigned int GAME_AREA00_EVENT_CONTROL;
extern volatile unsigned short PAD_PRESSED;
extern unsigned short BUTTON_MASK_A;
extern void playerSetFallLean(TObj *o);

void func_801057BC(TObj *o)
{
    char *p, *q;
    int t, v;
    volatile unsigned short *r;
    PLAYER_EXT[8] = 0;
    *((unsigned char *)o + 0xad) = 0;
    p = PLAYER_EXT;
    q = BITTEN_OBJECT;
    *(short *)((char *)o + 0xb2) = 0;
    *(short *)(p + 0x20) = 0xe;
    if (q[2] == 0x1c && (t = GAME_BITTEN_JUMP_KIND) < 3 && t != 0)
        *((unsigned char *)o + 0xab) |= 0x80;
    if ((GAME_AREA00_EVENT_CONTROL & 0x40) && (*(r = &PAD_PRESSED) & BUTTON_MASK_A))
        *((unsigned char *)o + 0xa7) = 1;
    v = 0x10;
    o->d84 = 0;
    if (o->animFrame & 1) {
        v = 0xf0;
        o->timer = 10;
    }
    o->step = 2;
    o->d88 = v;
    o->d8c = 0;
    o->state = 3;
}
#else
#include "TOBJ.H"
extern char *PLAYER_EXT;
extern char *BITTEN_OBJECT;
extern unsigned char GAME_BITTEN_JUMP_KIND;
extern unsigned int GAME_AREA00_EVENT_CONTROL;
extern volatile unsigned short PAD_PRESSED;
extern unsigned short BUTTON_MASK_A;
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
    if ((GAME_AREA00_EVENT_CONTROL & 0x40) && (*(r = &PAD_PRESSED) & BUTTON_MASK_A))
        *((unsigned char *)o + 0xa7) = 1;
    playerSetFallLean(o);
    o->step = 2;
    o->state = 3;
}
#endif
