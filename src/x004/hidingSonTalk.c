// FUNC 8012fbf4 1352 X004
// MATCHING 8012fbf4 1352
// VERSION SLPS_01144 x04 8012E394 1352
#if defined(VERSION_SLPS_01144)
#include "include_asm.h"
INCLUDE_ASM_V("x04/nonmatchings/x04", hidingSonTalk);
#else
#include "gcd.h"
#include "TOBJ.H"
typedef struct { char c[12]; } B12;
typedef struct { void **p; int x, y; } AT3;
extern TObj PLAYER;
extern unsigned char DAT_8009c942[], GAME_PLAYER_MODE;
extern unsigned char D_8009D13E;
extern short D_1F8001C6;
extern AT3 D_801314C8[];
extern TObj *showMessageBoxDirect(int, int, B12 *);
extern void AnimLoadDuration(TObj *);
extern void AnimAdvance(TObj *);
extern short FUN_80040278(TObj *, int, int);
extern void FUN_80026c50(int, int, int);
extern void setEventComplete(int, int);

void hidingSonTalk(TObj *o)
{
    B12 v;
    unsigned char *c;

    switch (o->state) {
    case 0:
        if (o->b68 == 0)
            break;
        v = *(B12 *)&o->a;
        GAME_PLAYER_MODE = 2;
        DAT_8009c942[0] = 1;
        PLAYER.b04 = 5;
        PLAYER.step = 0;
        PLAYER.state = 0;
        ((unsigned char*)&GCD.bcfcb)[0] = 1;
        o->wbc = o->animFrame;
        o->animFrame = o->b68 & 1;
        o->anim = D_801314C8[o->subtype].p[1];
        AnimLoadDuration(o);
        if (GCD.bce3b == 1) {
            o->d90 = (int)showMessageBoxDirect(2, 0xe, &v);
            o->state = 10;
        } else {
            o->d90 = (int)showMessageBoxDirect(2, 0xe, &v);
            o->state = 1;
        }
        break;
    case 1:
        AnimAdvance(o);
        if (((TObj *)o->d90)->b04 != 2)
            break;
        ((TObj *)o->d90)->b04 = 3;
        o->anim = D_801314C8[o->subtype].p[0];
        AnimLoadDuration(o);
        o->animFrame = 1;
        o->velX = -0x180;
        o->velY = -0x600;
        o->state++;
        break;
    case 2:
        AnimAdvance(o);
        o->h->raw += o->velX << 8;
        o->y.raw += o->velY << 8;
        o->velY += 0x40;
        if (o->velY > 0)
            o->state++;
        break;
    case 3:
        AnimAdvance(o);
        o->h->raw += o->velX << 8;
        o->y.raw += o->velY << 8;
        o->velY += 0x40;
        if (FUN_80040278(o, o->h->p.whole, o->y.p.whole)) {
            o->anim = D_801314C8[o->subtype].p[0];
            AnimLoadDuration(o);
            o->velX = -0x180;
            o->velY = -0x600;
            o->state = 2;
        }
        if (o->visible == 0)
            o->state = 9;
        break;
    case 9:
        DAT_8009c942[0] = 0;
        PLAYER.wb2 = 0;
        D_1F8001C6 = 0;
        o->animFrame = o->wbc;
        c = &GCD.bcfca;
        PLAYER.b04 = 1;
        PLAYER.step = 0;
        PLAYER.state = 0;
        (*c)++;
        o->b68 = 0;
        o->b04 = 3;
        o->step = 0;
        o->state = 0;
        break;
    case 10:
        AnimAdvance(o);
        if (((TObj *)o->d90)->b04 != 2)
            break;
        ((TObj *)o->d90)->b04 = 3;
        if (D_8009D13E == 0) {
            o->anim = D_801314C8[o->subtype].p[0];
            AnimLoadDuration(o);
            v = *(B12 *)&o->a;
            o->d90 = (int)showMessageBoxDirect(2, 0x10, &v);
            o->state = 11;
        } else {
            o->anim = D_801314C8[o->subtype].p[0];
            AnimLoadDuration(o);
            v = *(B12 *)&o->a;
            o->d90 = (int)showMessageBoxDirect(2, 0xf, &v);
            o->state = 12;
        }
        break;
    case 11:
        AnimAdvance(o);
        if (((TObj *)o->d90)->b04 != 2)
            break;
        ((TObj *)o->d90)->b04 = 3;
        o->state = 14;
        break;
    case 12:
        AnimAdvance(o);
        if (((TObj *)o->d90)->b04 != 2)
            break;
        ((TObj *)o->d90)->b04 = 3;
        FUN_80026c50(0x37, 1, 1);
        FUN_80026c50(0x37, 1, 1);
        o->state = 14;
        break;
    case 14:
        o->timer = 200;
        setEventComplete(0x97, 0);
        o->state++;
        break;
    case 15:
        if (--o->timer <= 0)
            o->state = 1;
        break;
    }
}
#endif
