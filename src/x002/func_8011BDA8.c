// FUNC 8011bda8 1860 X002
// MATCHING 8011bda8 1860
#include "gcd.h"
#include "gv2.h"
#include "gvars.h"
#include "TOBJ.H"
typedef struct { short v[6]; } V6;
typedef struct { char p[0x24]; unsigned short w24; } W24;
extern TObj *SCRIPT_OBJECTS;
extern TObj *D_8009F2DC;
extern void *D_8009F328;
extern void *D_8009F32C;
extern TObj PLAYER;
extern short D_800A604E[];
extern short D_1F8001C6;
extern int AnimAdvance(TObj *);
extern TObj *showMessageBoxDirect(int, int, V6 *);
extern void func_80117CEC(TObj *, int, int);
extern void func_80116928(void *, int, int);
extern void setEventComplete(int, int);
extern void FUN_800eea7c(TObj *, int, int);

void func_8011BDA8(TObj *o)
{
    TObj *a = SCRIPT_OBJECTS;
    TObj *b = D_8009F2DC;
    void *c = D_8009F328;
    void *d = D_8009F32C;
    V6 v;

    switch (o->state) {
    case 0:
        if (GVARS.b3a == 0) {
            break;
        }
        ((unsigned char*)&GVARS.b42)[0] = 1;
        ((unsigned char*)&GVARS.b3f)[0] = 1;
        GV2.bb0 = 2;
        PLAYER.animFrame = 1;
        PLAYER.b04 = 5;
        PLAYER.step = 0;
        PLAYER.state = 0;
        o->state++;
        break;
    case 1:
        o->w08 = 0x3c;
        v = *(V6 *)&a->a;
        v.v[3] -= 0x50;
        a->d90 = (int)showMessageBoxDirect(7, 9, &v);
        func_80117CEC(a, 2, 0);
        o->state++;
        break;
    case 2:
        AnimAdvance(a);
        {
            TObj *p = (TObj *)a->d90;
            if (p->b04 != 2) {
                break;
            }
            p->b04 = 3;
        }
        func_80117CEC(a, 1, 0);
        o->w08 = 0x3c;
        o->state++;
        break;
    case 3:
        if (--o->w08 > 0) {
            break;
        }
        b->animFrame = 1;
        b->h->p.whole = PLAYER.h->p.whole;
        b->y.p.whole = D_800A604E[0];
        {
            int x = b->h->p.whole;
            int y;
            b->velV = -0x200;
            b->velX = (0x28 - x) << 2;
            y = b->y.p.whole;
            b->velY = (-0x46 - y) << 2;
        }
        func_80117CEC(b, 4, 0);
        o->w08 = 0x40;
        o->state++;
        break;
    case 4:
        b->h->raw += b->velX << 8;
        b->y.raw += b->velY << 8;
        b->y.raw += b->velV << 8;
        b->velV += 0x10;
        if (--o->w08 > 0) {
            break;
        }
        b->b0f = 4;
        func_80117CEC(b, 0, 0);
        func_80116928(d, 3, 0);
        o->w08 = 0x3c;
        ((unsigned char*)&GCD.bcfdb)[0] = 1;
        o->state++;
        break;
    case 5:
        if (b->y.p.whole < -0x3e) {
            b->y.p.whole++;
        }
        if (--o->w08 > 0) {
            break;
        }
        setEventComplete(0x9d, 0);
        o->w08 = 0xc8;
        o->state++;
        break;
    case 6:
        if (--o->w08 > 0) {
            break;
        }
        v = *(V6 *)&a->a;
        v.v[3] -= 0x50;
        a->d90 = (int)showMessageBoxDirect(7, 10, &v);
        func_80117CEC(a, 2, 0);
        o->state++;
        break;
    case 7:
        AnimAdvance(a);
        {
            TObj *p = (TObj *)a->d90;
            if (p->b04 != 2) {
                break;
            }
            p->b04 = 3;
        }
        v = *(V6 *)&a->a;
        v.v[3] -= 0x50;
        a->d90 = (int)showMessageBoxDirect(3, 3, &v);
        o->state++;
        break;
    case 8:
        AnimAdvance(a);
        {
            TObj *p = (TObj *)a->d90;
            if (p->b04 != 2) {
                break;
            }
            p->b04 = 3;
        }
        v = *(V6 *)&a->a;
        v.v[3] -= 0x50;
        a->d90 = (int)showMessageBoxDirect(3, 4, &v);
        o->state++;
        break;
    case 9:
        AnimAdvance(a);
        {
            TObj *p = (TObj *)a->d90;
            if (p->b04 != 2) {
                break;
            }
            p->b04 = 3;
        }
        func_80116928(c, 1, 0);
        o->w08 = 0x78;
        o->state = 11;
        break;
    case 11:
        if (--o->w08 > 0) {
            break;
        }
        PLAYER.b04 = 5;
        PLAYER.step = 0x64;
        PLAYER.state = 0;
        FUN_800eea7c(&PLAYER, 1, 0);
        o->w08 = PLAYER.h->p.whole - 0xa0;
        o->state = 20;
        break;
    case 20:
        PLAYER.h->p.whole--;
        if (--o->w08 > 0) {
            break;
        }
        FUN_800eea7c(&PLAYER, 0x2b, 0);
        D_1F8001C6 = 0;
        o->w08 = 0x50;
        o->state++;
        break;
    case 21:
        PLAYER.d->p.whole += 8;
        if (--o->w08 > 0) {
            break;
        }
        {
            int y = b->y.p.whole;
            int x;
            x = b->h->p.whole;
            b->velY = (-0x46 - y) << 2;
            b->animFrame = 0;
            b->velX = (0xa0 - x) << 2;
            b->velV = -0x200;
        }
        func_80117CEC(b, 4, 0);
        o->w08 = 0x40;
        o->state++;
        break;
    case 22:
        PLAYER.d->p.whole += 8;
        b->h->raw += b->velX << 8;
        b->y.raw += b->velY << 8;
        b->y.raw += b->velV << 8;
        b->velV += 0x10;
        if (--o->w08 > 0) {
            break;
        }
        b->b04 = 3;
        o->state++;
        break;
    case 23:
        PLAYER.d->p.whole++;
        break;
    case 99:
        ((unsigned char*)&GVARS.b42)[0] = 0;
        ((unsigned char*)&GVARS.b3f)[0] = 0;
        ((unsigned char*)&GVARS.b3e)[0] = 0;
        PLAYER.wb2 = 0;
        D_1F8001C6 = 0;
        o->b0f = ((W24 *)o)->w24;
        PLAYER.b04 = 1;
        PLAYER.step = 0;
        PLAYER.state = 0;
        o->step = 0;
        o->state = 0;
        break;
    }
}
