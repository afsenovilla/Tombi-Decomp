// FUNC 8012866c 2856 X004
// MATCHING 8012866c 2856
#include "TOBJ.H"
typedef struct { short v[6]; } V6;
typedef struct { void **anims; int a; int b; } AT;
extern AT D_8013117C[];
extern TObj D_800A6038;
extern unsigned char D_8009CE1B;
extern unsigned char D_8009C93F, D_8009C942, D_8009C93E;
extern unsigned char D_800A6102, D_800A60D9;
extern unsigned char *D_8009C330;
extern void AnimAdvance(TObj *);
extern void AnimJump(TObj *, int);
extern int FUN_8002dcc8(int, int, V6 *);
extern void FUN_8003e300(int, int, Fix16 *);
extern void FUN_800eea7c(TObj *, int, int);
extern void FUN_8005a8a8(int, int, int);

void func_8012866C(TObj *o)
{
    V6 v;
    TObj *e;

    AnimAdvance(&D_800A6038);
    switch (o->state) {
    case 0:
        if (D_8009CE1B) {
            o->state = 0x32;
            break;
        }
        v.v[1] = 0;
        FUN_8003e300(0x7e, 0, (Fix16 *)&v);
        D_8009C93F = 1;
        D_8009C942 = 1;
        D_8009C93E = 0;
        D_800A6038.active = 4;
        D_800A6038.b04 = 5;
        D_800A6038.step = 0x65;
        D_800A6038.d8c = 0x100;
        D_800A6038.animFrame = 0;
        D_800A6038.state = 0;
        D_800A6038.timer = 0x64;
        FUN_800eea7c(&D_800A6038, 0x47, 0);
        o->timer = 0x3c;
        o->state++;
        break;
    case 1:
        if (--o->timer <= 0) {
            v = *(V6 *)&o->a;
            o->d90 = FUN_8002dcc8(4, 4, &v);
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
            o->d90 = FUN_8002dcc8(4, 5, &v);
            o->state++;
        }
        break;
    case 3:
        e = (TObj *)o->d90;
        if (e->b04 == 2) {
            e->b04 = 3;
            o->anim = D_8013117C[o->subtype].anims[0];
            AnimJump(o, 0);
            FUN_800eea7c(&D_800A6038, 0x44, 0);
            o->timer = 0x14;
            o->state++;
        }
        break;
    case 4:
        D_800A6038.d8c = (unsigned char)(D_800A6038.d8c - 1);
        if (o->timer != 0) {
            o->timer--;
            D_800A6038.y.p.whole++;
        }
        if (D_800A6038.d8c < 0xc0) {
            D_800A6038.d8c = 0xc0;
            v = *(V6 *)&o->a;
            o->d90 = FUN_8002dcc8(4, 6, &v);
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
        if (++D_800A6038.y.p.whole > -0x2c) {
            D_800A6038.y.p.whole = -0x2c;
            o->state++;
        }
        break;
    case 7:
        if (++D_800A6038.d8c >= 0x100) {
            D_800A6038.d8c = 0x100;
            o->timer = 0x14;
            v = *(V6 *)&o->a;
            o->d90 = FUN_8002dcc8(4, 7, &v);
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
        D_800A6038.h->p.whole += 1;
        if (--o->timer <= 0) {
            FUN_800eea7c(&D_800A6038, 0x45, 0);
            o->timer = 0x14;
            o->state++;
        }
        break;
    case 10:
        D_800A6038.h->p.whole += 2;
        if (--o->timer <= 0) {
            FUN_800eea7c(&D_800A6038, 0x44, 0);
            o->state++;
        }
        break;
    case 11:
        D_800A6038.h->p.whole += 1;
        if (D_800A6038.h->p.whole > 0xf0) {
            D_800A6038.active = 1;
            o->timer = 0xc8;
            o->state++;
        }
        break;
    case 12:
        if (--o->timer <= 0) {
            D_800A6038.animFrame = 1;
            o->timer = 0x14;
            o->state++;
        }
        break;
    case 13:
        D_800A6038.h->p.whole -= 1;
        if (--o->timer <= 0) {
            FUN_800eea7c(&D_800A6038, 0x45, 0);
            o->timer = 0x14;
            o->state++;
        }
        break;
    case 14:
        D_800A6038.h->p.whole -= 2;
        if (--o->timer <= 0) {
            FUN_800eea7c(&D_800A6038, 0x44, 0);
            o->state++;
        }
        break;
    case 15:
        D_800A6038.h->p.whole -= 1;
        if (D_800A6038.h->p.whole < 0x60) {
            D_800A6038.d8c = 0x100;
            D_800A6038.h->p.whole = 0x60;
            o->state++;
        }
        break;
    case 16:
        if (--D_800A6038.d8c < 0xc0) {
            o->state++;
        }
        break;
    case 17:
        if (--D_800A6038.y.p.whole < -0x98) {
            D_800A6038.animFrame = 0;
            D_800A6038.d8c = 0x40;
            o->state++;
        }
        break;
    case 18:
        if (--D_800A6038.d8c < 0) {
            D_800A6038.d8c = 0;
            o->timer = 0x3c;
            FUN_800eea7c(&D_800A6038, 0x47, 0);
            o->state++;
        }
        break;
    case 19:
        if (--o->timer <= 0) {
            D_800A6038.animFrame = 0;
            D_8009C330[8] = 0;
            D_8009C330[9] = 5;
            D_800A6038.b9c = 1;
            D_800A6038.velX = 0x1c0;
            D_800A6038.b04 = 6;
            D_800A6038.b69 = 0;
            D_800A6038.wb2 = 0;
            D_800A6038.velH = 0;
            D_800A6038.velV = 0;
            D_800A6038.velY = 0;
            D_800A6038.step = 4;
            D_800A6038.state = 0;
            o->state++;
        }
        break;
    case 20:
        if (D_800A6038.b69) {
            D_800A6038.b04 = 5;
            D_800A6038.step = 0;
            D_800A6038.state = 0;
            v = *(V6 *)&o->a;
            o->d90 = FUN_8002dcc8(4, 8, &v);
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
            o->d90 = FUN_8002dcc8(4, 9, &v);
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
            FUN_8005a8a8(0x77, 0, 0);
            o->timer = 0xc8;
            o->state++;
        }
        break;
    case 24:
        D_800A6038.active = 1;
        D_800A6038.b04 = 1;
        D_800A6102 = 0;
        D_800A60D9 = 0;
        D_8009C93F = 0;
        D_8009C942 = 0;
        D_8009C93E = 0;
        D_800A6038.wb2 = 0;
        D_800A6038.step = 0;
        D_800A6038.state = 0;
        o->animFrame = 1;
        o->b68 = 0;
        o->step = 0;
        o->state = 0;
        break;
    }
}
