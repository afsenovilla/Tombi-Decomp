// FUNC 800fa01c 832 X010
// MATCHING 800fa01c 832
#include "plx.h"
#include "gvars.h"
#include "TOBJ.H"
#include "raw7.h"
#include "gamestate.h"
extern TObj *PLAYER_EXT;
extern unsigned char *BITTEN_OBJECT;
extern unsigned short JOYPAD_STATE, BUTTON_MASK_B;
extern void ObjSetAnimFromTable(TObj *);
extern void AnimJump(TObj *, int);
extern void AnimAdvance(TObj *);
extern void SfxPlay3(int, int);
extern void probeCollisionAtDepthC(TObj *, int, int);
extern void playerCheckPerchLanding(TObj *);

static __inline__ void setanim(TObj *o, unsigned short anim)
{
    TObj *p = PLAYER_EXT;
    p->animTimer = anim;
    if (p->animFrame != anim) {
        p->animTimer = anim;
        ObjSetAnimFromTable(o);
        AnimJump(o, 0);
        PLAYER_EXT->animFrame = PLAYER_EXT->animTimer;
    }
}

typedef struct { unsigned char b0; char p1; short w2; char p4[8]; short wc; short we; char p10[0x28 - 0x10]; unsigned short w28, w2a; } PL;

void playerStepPerchIdle(TObj *o)
{
    PL *p;
    switch (o->state) {
    case 0:
        p = (PL *)PLAYER_EXT;
        o->wb6 = 0;
        U8(p, 8) = 0;
        p->we = 0;
        p->w2 = 0;
        (*(unsigned char*)((char*)&PLAYER_EXT->w08 + 1)) = 0;
        ((PL *)PLAYER_EXT)->wc = 0;
        ((PL *)PLAYER_EXT)->w28 = 0xffff;
        ((PL *)PLAYER_EXT)->w2a = 0xffff;
        o->velH = 0;
        o->velV = 0;
        o->wb0 = 0;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        o->h->p.whole = o->h->p.whole;
        o->y.p.whole += 4;
        if ((*(unsigned char*)&o->wac) >= 2) {
            BITTEN_OBJECT = (*(unsigned char **)&PLX.p611c);
            BITTEN_OBJECT[4] = 2;
            BITTEN_OBJECT[5] = 2;
            BITTEN_OBJECT[6] = 0;
        }
        (*(unsigned char*)&o->wac) = 0;
        GVARS.i34 = 0;
        U8(o, 0xc7) = 1;
        o->b9d = 0;
        U8(o, 0xc6) = 0;
        U8(o, 0xe3) = 0;
        PLAYER_EXT->active = 0;
        setanim(o, 7);
        SfxPlay3(4, 0x7f);
        o->state++;
    case 1:
        AnimAdvance(o);
        if ((*(int*)&GAME.selectedArea) == 0x30000) break;
        if (JOYPAD_STATE & BUTTON_MASK_B) {
            (*(unsigned char*)((char*)&PLAYER_EXT->w08 + 1)) = 0;
            setanim(o, 9);
            o->b04 = 1;
            o->step = 0xb;
            o->state = 0;
        }
        if (JOYPAD_STATE & 0x10) {
            (*(unsigned char*)((char*)&PLAYER_EXT->w08 + 1)) = 0;
            setanim(o, 9);
            o->b04 = 1;
            o->step = 0xb;
            o->state = 0;
        }
        if (JOYPAD_STATE & 0x40) {
            (*(unsigned char*)((char*)&PLAYER_EXT->w08 + 1)) = 0;
            o->y.p.whole += 9;
            o->h->p.whole += (o->animFrame & 1) ? -4 : 4;
            probeCollisionAtDepthC(o, o->h->p.whole, o->y.p.whole);
            o->step = 0xd;
            o->state = 0;
        }
        break;
    }
    playerCheckPerchLanding(o);
}
