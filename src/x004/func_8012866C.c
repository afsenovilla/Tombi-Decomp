// FUNC 8012866c 2856 X004
// MATCHING 8012866c 2856
#include "plx.h"
#include "gvars.h"
#include "TOBJ.H"
typedef struct { short v[6]; } V6;
typedef struct { void **anims; int a; int b; } AT;
extern AT D_8013117C[];
extern TObj PLAYER;
extern unsigned char D_8009CE1B;
extern unsigned char D_800A60D9;
extern unsigned char *D_8009C330;
extern void AnimAdvance(TObj *);
extern void AnimJump(TObj *, int);
extern int showMessageBoxDirect(int, int, V6 *);
extern void FUN_8003e300(int, int, Fix16 *);
extern void FUN_800eea7c(TObj *, int, int);
extern void setEventStarted(int, int, int);

void func_8012866C(TObj *o)
{
    V6 v;
    TObj *e;

    AnimAdvance(&PLAYER);
    switch (o->state) {
    case 0:
        if (D_8009CE1B) {
            o->state = 0x32;
            break;
        }
        v.v[1] = 0;
        FUN_8003e300(0x7e, 0, (Fix16 *)&v);
        GVARS.b3f = 1;
        GVARS.b42 = 1;
        GVARS.b3e = 0;
        PLAYER.active = 4;
        PLAYER.b04 = 5;
        PLAYER.step = 0x65;
        PLAYER.d8c = 0x100;
        PLAYER.animFrame = 0;
        PLAYER.state = 0;
        PLAYER.timer = 0x64;
        FUN_800eea7c(&PLAYER, 0x47, 0);
        o->timer = 0x3c;
        o->state++;
        break;
    case 1:
        if (--o->timer <= 0) {
            v = *(V6 *)&o->a;
            o->d90 = showMessageBoxDirect(4, 4, &v);
            o->anim = D_8013117C[o->subtype].anims[2];
            AnimJump(o, 0);
            o->state++;
        }
        break;
    case 2:
        e = (TObj *)o->d90;
        if (e->b04 == 2) {
            e->b04 = 3;
            v = *(V6 *)&o->a;
            o->d90 = showMessageBoxDirect(4, 5, &v);
            o->state++;
        }
        break;
    case 3:
        e = (TObj *)o->d90;
        if (e->b04 == 2) {
            e->b04 = 3;
            o->anim = D_8013117C[o->subtype].anims[0];
            AnimJump(o, 0);
            FUN_800eea7c(&PLAYER, 0x44, 0);
            o->timer = 0x14;
            o->state++;
        }
        break;
    case 4:
        PLAYER.d8c = (unsigned char)(PLAYER.d8c - 1);
        if (o->timer != 0) {
            o->timer--;
            PLAYER.y.p.whole++;
        }
        if (PLAYER.d8c < 0xc0) {
            PLAYER.d8c = 0xc0;
            v = *(V6 *)&o->a;
            o->d90 = showMessageBoxDirect(4, 6, &v);
            o->anim = D_8013117C[o->subtype].anims[2];
            AnimJump(o, 0);
            o->state++;
        }
        break;
    case 5:
        e = (TObj *)o->d90;
        if (e->b04 == 2) {
            e->b04 = 3;
            o->anim = D_8013117C[o->subtype].anims[0];
            AnimJump(o, 0);
            o->state++;
        }
        break;
    case 6:
        if (++PLAYER.y.p.whole > -0x2c) {
            PLAYER.y.p.whole = -0x2c;
            o->state++;
        }
        break;
    case 7:
        if (++PLAYER.d8c >= 0x100) {
            PLAYER.d8c = 0x100;
            o->timer = 0x14;
            v = *(V6 *)&o->a;
            o->d90 = showMessageBoxDirect(4, 7, &v);
            o->anim = D_8013117C[o->subtype].anims[2];
            AnimJump(o, 0);
            o->state++;
        }
        break;
    case 8:
        e = (TObj *)o->d90;
        if (e->b04 == 2) {
            e->b04 = 3;
            o->anim = D_8013117C[o->subtype].anims[0];
            AnimJump(o, 0);
            o->state++;
        }
        break;
    case 9:
        PLAYER.h->p.whole += 1;
        if (--o->timer <= 0) {
            FUN_800eea7c(&PLAYER, 0x45, 0);
            o->timer = 0x14;
            o->state++;
        }
        break;
    case 10:
        PLAYER.h->p.whole += 2;
        if (--o->timer <= 0) {
            FUN_800eea7c(&PLAYER, 0x44, 0);
            o->state++;
        }
        break;
    case 11:
        PLAYER.h->p.whole += 1;
        if (PLAYER.h->p.whole > 0xf0) {
            PLAYER.active = 1;
            o->timer = 0xc8;
            o->state++;
        }
        break;
    case 12:
        if (--o->timer <= 0) {
            PLAYER.animFrame = 1;
            o->timer = 0x14;
            o->state++;
        }
        break;
    case 13:
        PLAYER.h->p.whole -= 1;
        if (--o->timer <= 0) {
            FUN_800eea7c(&PLAYER, 0x45, 0);
            o->timer = 0x14;
            o->state++;
        }
        break;
    case 14:
        PLAYER.h->p.whole -= 2;
        if (--o->timer <= 0) {
            FUN_800eea7c(&PLAYER, 0x44, 0);
            o->state++;
        }
        break;
    case 15:
        PLAYER.h->p.whole -= 1;
        if (PLAYER.h->p.whole < 0x60) {
            PLAYER.d8c = 0x100;
            PLAYER.h->p.whole = 0x60;
            o->state++;
        }
        break;
    case 16:
        if (--PLAYER.d8c < 0xc0) {
            o->state++;
        }
        break;
    case 17:
        if (--PLAYER.y.p.whole < -0x98) {
            PLAYER.animFrame = 0;
            PLAYER.d8c = 0x40;
            o->state++;
        }
        break;
    case 18:
        if (--PLAYER.d8c < 0) {
            PLAYER.d8c = 0;
            o->timer = 0x3c;
            FUN_800eea7c(&PLAYER, 0x47, 0);
            o->state++;
        }
        break;
    case 19:
        if (--o->timer <= 0) {
            PLAYER.animFrame = 0;
            D_8009C330[8] = 0;
            D_8009C330[9] = 5;
            PLAYER.b9c = 1;
            PLAYER.velX = 0x1c0;
            PLAYER.b04 = 6;
            PLAYER.b69 = 0;
            PLAYER.wb2 = 0;
            PLAYER.velH = 0;
            PLAYER.velV = 0;
            PLAYER.velY = 0;
            PLAYER.step = 4;
            PLAYER.state = 0;
            o->state++;
        }
        break;
    case 20:
        if (PLAYER.b69) {
            PLAYER.b04 = 5;
            PLAYER.step = 0;
            PLAYER.state = 0;
            v = *(V6 *)&o->a;
            o->d90 = showMessageBoxDirect(4, 8, &v);
            o->anim = D_8013117C[o->subtype].anims[2];
            AnimJump(o, 0);
            o->state++;
        }
        break;
    case 21:
        e = (TObj *)o->d90;
        if (e->b04 == 2) {
            e->b04 = 3;
            o->anim = D_8013117C[o->subtype].anims[0];
            AnimJump(o, 0);
            o->timer = 0x1e;
            o->state++;
        }
        break;
    case 22:
        if (--o->timer <= 0) {
            v = *(V6 *)&o->a;
            o->d90 = showMessageBoxDirect(4, 9, &v);
            o->anim = D_8013117C[o->subtype].anims[2];
            AnimJump(o, 0);
            o->state++;
        }
        break;
    case 23:
        e = (TObj *)o->d90;
        if (e->b04 == 2) {
            e->b04 = 3;
            o->anim = D_8013117C[o->subtype].anims[0];
            AnimJump(o, 0);
            setEventStarted(0x77, 0, 0);
            o->timer = 0xc8;
            o->state++;
        }
        break;
    case 24:
        PLAYER.active = 1;
        PLAYER.b04 = 1;
        PLX.b6102 = 0;
        D_800A60D9 = 0;
        GVARS.b3f = 0;
        GVARS.b42 = 0;
        GVARS.b3e = 0;
        PLAYER.wb2 = 0;
        PLAYER.step = 0;
        PLAYER.state = 0;
        o->animFrame = 1;
        o->b68 = 0;
        o->step = 0;
        o->state = 0;
        break;
    }
}
