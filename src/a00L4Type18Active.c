// FUNC 8011f9e0 1924 X000
// MATCHING 8011f9e0 1924
#include "plx.h"
#include "gvars.h"
#include "TOBJ.H"
#include "player.h"

typedef struct { unsigned char p0[9]; unsigned char b9; } P8011F9E0;

extern P8011F9E0 *PLAYER_EXT;
extern Fix16 *D_800A6078_[];
#define D_800A6078 D_800A6078_[0]
extern void *A00_L4_TYPE18_ANIMS;
extern void *D_8013B150;
extern void *D_8013B154;
extern unsigned char D_800A603C_[], D_800A603D_[], D_800A603E_[];
#define D_800A603C D_800A603C_[0]
#define D_800A603D D_800A603D_[0]
#define D_800A603E D_800A603E_[0]
extern unsigned char D_800A60A1_[];
#define D_800A60A1 D_800A60A1_[0]
extern unsigned char D_8009CEBF;
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern short FUN_8001fdac(int, int);
extern void setEventStarted(int, int, int);

#define BOB() \
    o->d88 += 4; \
    o->y.p.whole = o->velX + FUN_8001fdac(o->d88 & 0xff, 4);

void a00L4Type18Active(TObj *o)
{
    TObj *p;
    unsigned char *k;

    switch (o->state) {
    case 0:
        o->d88 = 0;
        o->anim = A00_L4_TYPE18_ANIMS;
        o->velH = o->h->p.whole - D_800A6078->p.whole;
        o->velV = 0;
        AnimLoadDuration(o);
        o->state++;
    case 1:
        o->h->p.whole++;
        D_800A6078->p.whole = o->h->p.whole;
        p = (TObj *)o->d90;
        p->h->p.whole = D_800A6078->p.whole - 6;
        BOB();
        AnimAdvance(o);
        AnimAdvance(p);
        if (o->h->p.whole >= 0x237) {
            k = &D_800A603D;
            if (*k != 4) {
                p = (TObj *)o->d90;
                p->anim = D_8013B150;
                AnimLoadDuration(p);
                o->h->p.whole = 0x236;
                p->h->p.whole = o->h->p.whole - 5;
                o->b69 = 0;
                o->timer = 0x20;
                o->state++;
                PLAYER.obj.animFrame = 0;
                PLAYER_EXT->b9 = 0x14;
                PLAYER.obj.b9c = 1;
                PLAYER.obj.velX = 0x130;
                D_800A603C = 6;
                D_800A60A1 = 0;
                PLAYER.obj.wb2 = 0;
                PLAYER.obj.velH = 0;
                PLAYER.obj.velV = 0;
                PLAYER.obj.velY = 0;
                *k = 4;
                D_800A603E = 0;
            }
        }
        break;
    case 2:
        p = (TObj *)o->d90;
        AnimAdvance(o);
        AnimAdvance(p);
        BOB();
        if (D_800A60A1) {
            D_800A603C = 5;
            D_800A603D = 0;
            D_800A603E = 0;
            o->timer = 0x3c;
            o->state = 3;
            o->velX = o->y.p.whole;
        }
        break;
    case 3:
        p = (TObj *)o->d90;
        AnimAdvance(o);
        AnimAdvance(p);
        BOB();
        o->velX++;
        if (--o->timer > 0) {
            if (o->timer == 0x14) {
                p = (TObj *)o->d90;
                p->anim = D_8013B154;
                AnimLoadDuration(p);
            }
        } else {
            p = (TObj *)o->d90;
            p->b0f = 0x1e;
            o->state++;
        }
    case 4:
        p = (TObj *)o->d90;
        AnimAdvance(o);
        AnimAdvance(p);
        BOB();
        if (D_800A603D == 3) {
            o->timer = 0x3c;
            o->state++;
        }
        break;
    case 5:
        if (--o->timer == 0) {
            p = (TObj *)o->d90;
            p->y.p.whole += 4;
            o->timer = 0x3a;
            o->state++;
        }
        break;
    case 6:
        AnimAdvance((TObj *)o->d90);
        AnimAdvance(o);
        BOB();
        o->velX--;
        if (--o->timer > 0) {
            if (o->timer == 0x28) {
                setEventStarted(8, 0, 0);
                D_800A603C = 5;
                D_800A603D = 0;
                D_800A603E = 0;
            }
            if (o->timer == 0x14) {
                p = (TObj *)o->d90;
                p->anim = D_8013B150;
                AnimLoadDuration(p);
                *(signed char *)&p->b0f = -10;
            }
        } else {
            (*(TObj **)&PLX.p611c)->b04 = 3;
            o->state++;
        }
        break;
    case 7:
        AnimAdvance((TObj *)o->d90);
        AnimAdvance(o);
        BOB();
        if (D_8009CEBF == 3) {
            o->timer = 0x3c;
            o->state++;
        }
        break;
    case 8:
        AnimAdvance((TObj *)o->d90);
        AnimAdvance(o);
        BOB();
        if (--o->timer == 0) {
            *(volatile unsigned short *)&PLAYER.obj.animFrame ^= 1;
            p = (TObj *)o->d90;
            p->animFrame ^= 1;
            o->state++;
        }
        break;
    case 9:
        AnimAdvance((TObj *)o->d90);
        AnimAdvance(o);
        o->h->p.whole--;
        D_800A6078->p.whole = o->h->p.whole;
        p = (TObj *)o->d90;
        p->h->p.whole = D_800A6078->p.whole + 6;
        BOB();
        if (o->h->p.whole < 0x129) {
            PLAYER.obj.animFrame = 1;
            PLAYER_EXT->b9 = 0xe;
            PLAYER.obj.b9c = 1;
            PLAYER.obj.velX = -0x100;
            D_800A603C = 6;
            D_800A60A1 = 0;
            PLAYER.obj.velY = 0;
            D_800A603D = 4;
            D_800A603E = 0;
            o->timer = 0x40;
            o->state++;
        }
        break;
    case 10:
        p = (TObj *)o->d90;
        AnimAdvance(o);
        AnimAdvance(p);
        BOB();
        o->velX++;
        if (--o->timer > 0) {
            if (o->timer == 0x14) {
                p = (TObj *)o->d90;
                p->anim = D_8013B154;
                AnimLoadDuration(p);
            }
        } else {
            p = (TObj *)o->d90;
            p->b04 = 2;
            o->b04 = 2;
            D_800A603C = 1;
            D_800A603D = 0;
            D_800A603E = 0;
            GVARS.b3f = 0;
        }
        break;
    }
}
