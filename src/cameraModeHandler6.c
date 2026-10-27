// FUNC 8002a3d4 252 MAIN0
// MATCHING 8002a3d4 252
#include "TOBJ.H"
/* PLAYER+0: a separate symbol on purpose (sharing PLAYER's lui changes the register allocation) */
extern unsigned char DAT_800a6038;
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
    if (DAT_800a6038 < 4 || DAT_800a6038 == 7) {
        b = *(short *)(o->d34 + 2);
        if (PLAYER_H[1] < (a = b - 0x90)) PLAYER_H[1] = a;
        else if ((a = b + 0x90) < PLAYER_H[1]) PLAYER_H[1] = a;
        if (GAME == 0x2000e) {
            lim((short *)(g + 0x16), *(short *)&o->d30, 0x5c);
        } else {
            lim((short *)(g + 0x16), *(short *)&o->d30, 0x90);
        }
    }
}
