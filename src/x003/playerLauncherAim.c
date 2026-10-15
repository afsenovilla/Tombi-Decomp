// FUNC 8011b870 452 X003
// MATCHING 8011b870 452
#include "spad.h"
#include "TOBJ.H"

extern void *D_80139864[];
extern unsigned short D_8007A1F0[];
extern unsigned char PLAYER[], D_800A603C[], D_800A603D[], D_800A603E[];
extern signed char D_8009D2B0[];
extern void AnimLoadDuration(TObj *);
extern void playerLauncherTurnToward(TObj *, int, int);
extern int playerLauncherCheckCatch(TObj *);

static __inline__ int near(TObj *o)
{
    unsigned short t;
    short lim = 0xc8;
    if (PLAYER[0] == 2) return 0;
    t = (*(unsigned short*)((char*)&SPAD.i170 + 2)) - o->d->p.whole + 0x2d;
    if (t >= 0x5b) return 0;
    t = (*(unsigned short*)((char*)&SPAD.i168 + 2)) - o->h->p.whole + 0x64;
    if (t > lim) return 0;
    t = (*(unsigned short*)((char*)&SPAD.i16c + 2)) - o->y.p.whole + 0x78;
    return t <= lim - 0x10;
}

void playerLauncherAim(TObj *o)
{
    switch (o->state) {
    case 0:
        o->state++;
        o->wac = 0;
        o->anim = D_80139864[0];
        AnimLoadDuration(o);
        break;
    case 1:
        o->ba7 += 2;
        if (o->ba7 > 0x80) o->ba7 = 0;
        { short t = (short)D_8007A1F0[o->ba7] >> 6; o->ba6 = t; }
        playerLauncherTurnToward(o, 0, 0);
        if (playerLauncherCheckCatch(o)) {
            o->b0a = 2;
            o->velH = 0x80;
            D_800A603C[0] = 1;
            D_8009D2B0[0] = 0;
            PLAYER[0] = 2;
            D_800A603D[0] = 0x36;
            D_800A603E[0] = 0;
            o->state = 0;
            o->step++;
        } else if (!near(o)) {
            o->step = 0;
            o->state = 0;
        }
        break;
    }
}
