// VERSION BETA_980203 beta980203 8002C72C 252
#if defined(VERSION_BETA_980203)
// Beta version: SCUS_94236 C of src/cameraModeHandler6_scus94236.c (cameraModeHandler6, twin score 1.000) [-O2 -G0] (tools/demo_sweep.py)
// USA version of PAL cameraModeHandler6.c (src/cameraModeHandler6.c): 0x5c);=>0x54);; o->d30, 0x90)=>o->d30, 0x88).
#include "TOBJ.H"
/* PLAYER+0: a separate symbol on purpose (sharing PLAYER's lui changes the register allocation) */
extern unsigned char D_800A5398;
extern unsigned char PLAYER[];
extern short *PLAYER_H;
extern int GAME;
extern void cameraFollowPlayerX(TObj *);
extern void cameraFollowPlayerY(TObj *);
static __inline__ void lim(short *q, short x, int k) { if (*q < x - k) *q = x - k; }
void cameraModeHandler6(TObj *o)
{
    short a, b;
    unsigned char *g;
    cameraFollowPlayerX(o);
    cameraFollowPlayerY(o);
    g = PLAYER;
    if (D_800A5398 < 4 || D_800A5398 == 7) {
        b = *(short *)(o->d34 + 2);
        if (PLAYER_H[1] < (a = b - 0x90)) PLAYER_H[1] = a;
        else if ((a = b + 0x90) < PLAYER_H[1]) PLAYER_H[1] = a;
        if (GAME == 0x2000e) {
            lim((short *)(g + 0x16), *(short *)&o->d30, 0x54);
        } else {
            lim((short *)(g + 0x16), *(short *)&o->d30, 0x88);
        }
    }
}
#endif
