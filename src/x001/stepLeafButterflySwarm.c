// FUNC 8013b51c 1116 X001
// MATCHING 8013b51c 1116
#include "gcd.h"
#include "gvars.h"
#include "TOBJ.H"
#include "gamestate.h"

extern unsigned short PLAYER_X[];
extern TObj PLAYER;
extern char D_80010748[];
extern Fix16 *PLAYER_H[];
extern short PLAYER_Y[];
extern TObj *D_8009F2EC[];
extern TObj *TASK_C;
extern int FUN_8001f9e0(void);
extern void spawnDebrisObjectB(TObj *, char, int, int, short);
extern void FUN_8001fe6c(void *);
extern int FUN_8001fdac(int, int);

#define S16(o, off) (*(short *)((char *)(o) + (off)))

void stepLeafButterflySwarm(TObj *o)
{
    TObj *e = *(TObj **)&o->category;
    TObj *p;
    short i;
    short n;
    int a, b;

    switch (o->step) {
    case 0:
        o->step++;
    case 1:
        if (GCD.bcee2) {
            n = ((unsigned char*)&GCD.bd0b4)[0];
            if (n > 30) n = 30;
            for (i = 0; i < n; i++) {
                a = FUN_8001f9e0() & 3;
                b = FUN_8001f9e0() & 3;
                spawnDebrisObjectB(e, 1, (short)(PLAYER_X[0] + a), (short)(PLAYER_X[2] - b), (short)(PLAYER_X[4] - 0x10));
            }
            GCD.bcee1 = 1;
            GCD.bcee2 = 0;
        }
        if (GCD.bce56 != 2) break;
        GVARS.b3f = 1;
        GVARS.b3e = 1;
        GVARS.b42 = 1;
        PLAYER.anim = D_80010748;
        FUN_8001fe6c(&PLAYER);
        o->a.raw = e->h->raw;
        {
            int y = e->y.raw;
            o->step++;
            o->w08 = 0x3c;
            o->y.raw = y;
        }
        break;
    case 2:
        e->h->raw = o->a.raw + ((((FUN_8001f9e0() & 3) << 8) - 0x180) << 8);
        e->y.raw = o->y.raw + ((((FUN_8001f9e0() & 7) << 8) - 0x380) << 8);
        if (o->w08 != 0 && --o->w08 <= 0) GCD.bce56 = 3;
        if (GCD.bce56 != 4) break;
        e->h->raw = o->a.raw;
        e->y.raw = o->y.raw;
        o->y.raw = 0;
        p = D_8009F2EC[0];
        (*(short*)&o->d34) = PLAYER_H[0]->p.whole - e->h->p.whole;
        (*(short*)((char*)&o->d34 + 2)) = PLAYER_Y[0] - e->y.p.whole;
        (*(short*)&o->d38) = p->h->p.whole - e->h->p.whole;
        (*(short*)((char*)&o->d38 + 2)) = p->y.p.whole - e->y.p.whole;
        (*(short*)&o->b0a) = 0;
        o->step++;
        break;
    case 3:
        if (--(*(short*)&o->b0a) > 0) break;
        (*(short*)&o->b0a) = 0x3c;
        o->step++;
        break;
    case 4:
        o->w08 = (o->w08 + 2) & 0xff;
        e->h->raw = o->a.raw + ((FUN_8001fdac(o->w08, 0x40) << 16) >> 8);
        e->y.raw += o->y.raw << 8;
        PLAYER_H[0]->p.whole = e->h->p.whole + (*(short*)&o->d34);
        p = D_8009F2EC[0];
        PLAYER_Y[0] = e->y.p.whole + (*(short*)((char*)&o->d34 + 2));
        p->h->p.whole = e->h->p.whole + (*(short*)&o->d38);
        p->y.p.whole = e->y.p.whole + (*(short*)((char*)&o->d38 + 2));
        o->y.raw -= 8;
        if (--(*(short*)&o->b0a) > 0) break;
        GCD.bcfcf = 1;
        GAME.fadeScreenControl = 3;
        o->step++;
        break;
    case 5:
        if (GAME.fadeScreenControl != 1) break;
        (*(short*)&GAME.nextArea) = 0x13;
        (*(short*)&GAME.nextSection) = 2;
        (*(short*)&GAME.nextSpawnPoint) = 1;
        {
            TObj *q = TASK_C;
            q->w4c = 7;
            q->w4e = 0;
        }
        o->step++;
        break;
    }
}
