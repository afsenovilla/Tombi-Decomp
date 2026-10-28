// FUNC 80116398 840 X013
// MATCHING 80116398 840
// VERSION SLPS_01144 x13 80115694 840
#if defined(VERSION_SLPS_01144)
// Japanese version of PAL x013 80116398 (x013/func_80116398.c): 242 -> 226.
#include "gcd.h"
#include "TOBJ.H"
#include "gamestate.h"
typedef struct {
    TObj t;
    unsigned char pad[0xe2 - 0xc0];
    short wf2;
} PL;
extern PL PLAYER;
extern signed char GAME_PLAYER_MODE;
extern int FUN_800202b4(TObj *);
extern short FUN_80040278(TObj *, short, short);
extern void playerSetAnim(TObj *, short, short);
extern void FUN_80049ca4(TObj *, int);
extern void FUN_8001fec0(TObj *);

void func_80116398(TObj *o)
{
    PL *p;

    FUN_800202b4(o);
    p = &PLAYER;
    switch (o->state) {
    case 0:
        if ((*(int *)&p->t.b04 & 0xffffff) != 0x30405) break;
        o->h->p.whole = p->t.h->p.whole;
        p->t.h->p.whole = o->h->p.whole;
        p->t.y.p.whole = o->y.p.whole - 0x14;
        *(signed char *)&o->b0f = -10;
        p->t.active = 5;
        p->t.visible = 1;
        p->t.b04 = 5;
        p->t.step = 0x41;
        p->t.state = 0;
        playerSetAnim(&p->t, 0x2c, 0);
        FUN_80049ca4(o, 1);
        o->velV = 0x500;
        o->state++;
        break;
    case 1:
        o->velV -= 8;
        if (o->velV < 0x100) o->velV = 0x100;
        o->y.raw += o->velV << 8;
        p->t.y.raw += o->velV << 8;
        if (o->y.p.whole < p->wf2 - 0x1e) break;
        if (!FUN_80040278(o, o->h->p.whole, o->y.p.whole + 0x10)) break;
        o->timer = 0x1e;
        o->state++;
        FUN_80049ca4(o, 0);
        break;
    case 2:
        if (--o->timer != -1) break;
        o->state++;
        break;
    case 3:
        p->t.y.raw += 0x8000;
        if (!FUN_80040278(&p->t, p->t.h->p.whole, p->t.y.p.whole + 0x14)) break;
        o->state++;
        FUN_80049ca4(o, 0);
        break;
    case 4:
        o->timer = 0x1e;
        p->t.animFrame = 1;
        playerSetAnim(&p->t, 1, 0);
        o->state++;
        GCD.bcff9 = 0;
        o->active = 2;
        break;
    case 5:
        FUN_8001fec0(&p->t);
        p->t.h->p.whole--;
        if (--o->timer != -1) break;
        o->timer = 0x1e;
        o->state++;
        p->t.animFrame = 1;
        playerSetAnim(&p->t, 0, 0);
        break;
    case 6:
        if (--o->timer != -1) break;
        o->state++;
        GAME_PLAYER_MODE = 0;
        p->t.b04 = 5;
        p->t.step = 0x65;
        p->t.state = 0;
        break;
    case 7:
        if (GAME.fadeScreenControl == 1) o->b04 = 3;
        break;
    }
}
#else
#include "gcd.h"
#include "TOBJ.H"
#include "gamestate.h"
typedef struct {
    TObj t;
    unsigned char pad[0xf2 - 0xc0];
    short wf2;
} PL;
extern PL PLAYER;
extern signed char GAME_PLAYER_MODE;
extern int FUN_800202b4(TObj *);
extern short FUN_80040278(TObj *, short, short);
extern void playerSetAnim(TObj *, short, short);
extern void FUN_80049ca4(TObj *, int);
extern void FUN_8001fec0(TObj *);

void func_80116398(TObj *o)
{
    PL *p;

    FUN_800202b4(o);
    p = &PLAYER;
    switch (o->state) {
    case 0:
        if ((*(int *)&p->t.b04 & 0xffffff) != 0x30405) break;
        o->h->p.whole = p->t.h->p.whole;
        p->t.h->p.whole = o->h->p.whole;
        p->t.y.p.whole = o->y.p.whole - 0x14;
        *(signed char *)&o->b0f = -10;
        p->t.active = 5;
        p->t.visible = 1;
        p->t.b04 = 5;
        p->t.step = 0x41;
        p->t.state = 0;
        playerSetAnim(&p->t, 0x2c, 0);
        FUN_80049ca4(o, 1);
        o->velV = 0x500;
        o->state++;
        break;
    case 1:
        o->velV -= 8;
        if (o->velV < 0x100) o->velV = 0x100;
        o->y.raw += o->velV << 8;
        p->t.y.raw += o->velV << 8;
        if (o->y.p.whole < p->wf2 - 0x1e) break;
        if (!FUN_80040278(o, o->h->p.whole, o->y.p.whole + 0x10)) break;
        o->timer = 0x1e;
        o->state++;
        FUN_80049ca4(o, 0);
        break;
    case 2:
        if (--o->timer != -1) break;
        o->state++;
        break;
    case 3:
        p->t.y.raw += 0x8000;
        if (!FUN_80040278(&p->t, p->t.h->p.whole, p->t.y.p.whole + 0x14)) break;
        o->state++;
        FUN_80049ca4(o, 0);
        break;
    case 4:
        o->timer = 0x1e;
        p->t.animFrame = 1;
        playerSetAnim(&p->t, 1, 0);
        o->state++;
        GCD.bcff9 = 0;
        o->active = 2;
        break;
    case 5:
        FUN_8001fec0(&p->t);
        p->t.h->p.whole--;
        if (--o->timer != -1) break;
        o->timer = 0x1e;
        o->state++;
        p->t.animFrame = 1;
        playerSetAnim(&p->t, 0, 0);
        break;
    case 6:
        if (--o->timer != -1) break;
        o->state++;
        GAME_PLAYER_MODE = 0;
        p->t.b04 = 5;
        p->t.step = 0x65;
        p->t.state = 0;
        break;
    case 7:
        if (GAME.fadeScreenControl == 1) o->b04 = 3;
        break;
    }
}
#endif
