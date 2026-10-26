// FUNC 8011e688 1600 X009
// MATCHING 8011e688 1600
#include "spad.h"
#include "gv2.h"
#include "TOBJ.H"
#include "raw7.h"
#include "gamestate.h"
extern TObj *DAT_8009c330;
extern TObj *DAT_8009f0ec;
extern unsigned char DAT_801152e8[];
extern volatile unsigned short PAD_PRESSED[];
extern void FUN_800eeb5c(TObj *, int);
extern void FUN_8001e560(int, int);
extern void FUN_8001e5f4(int, int);
extern short FUN_8003fd78(TObj *, int, int);
extern void playerSetFallLean(TObj *);
extern int columnProbeA(TObj *, short, short, int);
extern short func_801215C0(TObj *, short, short, int);
extern short columnProbeB(TObj *, short, short, int);

void playerStep25Area9(TObj *o)
{
    TObj *p;
    short x, s;
    int v;
    short yy;
    unsigned short dx, dy;
    volatile unsigned short *k;

    o->h->p.whole += DAT_8009f0ec->velX;
    o->y.p.whole += DAT_8009f0ec->velY;
    switch (o->state) {
    case 0:
        FUN_800eeb5c(o, 10);
        o->d8c = 0;
        o->wb6 = 0;
        (*(unsigned short*)&DAT_8009c330->type) = 0;
        o->wb2 = 0;
        o->velH = 0;
        o->velV = 0;
        (*(unsigned char*)&o->waa) = 1;
        o->state++;
    case 1:
        k = PAD_PRESSED;
        s = 0;
        if (!(*k & 0x50)) {
            FUN_800eeb5c(o, 10);
        } else if (*k & 0x10) {
            FUN_800eeb5c(o, 0xb);
            v = o->h->p.whole;
            if (o->animFrame & 1)
                x = v - 8;
            else
                x = v + 8;
            yy = o->y.p.whole;
            p = DAT_8009f0ec;
            dx = x - (p->h->p.whole - p->box0);
            dy = p->y.p.whole + (p->box3 - p->box2);
            if ((unsigned short)dx < p->box1) {
                int c = -0x15;
                s += columnProbeA(o, dx, yy + c - dy, p->subtype);
            }
            p = DAT_8009f0ec;
            dx = x - (p->h->p.whole - p->box0);
            dy = p->y.p.whole + (p->box3 - p->box2);
            if ((unsigned short)dx < p->box1) {
                int c = -0x15;
                s += columnProbeA(o, dx, o->y.p.whole + c - dy, p->subtype);
            }
            if (s == 0)
                o->y.raw += -0x18000;
            if ((SPAD.s1f8 & 0xf) == 0)
                FUN_8001e560(0x1d, 0);
        } else if (*k & 0x40) {
            FUN_800eeb5c(o, 0xc);
            if ((SPAD.s1f8 & 0xf) == 0)
                FUN_8001e560(0x1d, 0);
            o->y.raw += 0x28000;
            if (o->b69) {
                o->d8c = DAT_801152e8[o->wb0];
                (*(unsigned char*)&o->waa) = 0;
                o->b9e = 0;
                o->step = 0;
                o->state = 0;
                break;
            }
            v = o->h->p.whole;
            if (o->animFrame & 1)
                x = v + 8;
            else
                x = v - 8;
            yy = o->y.p.whole + 0x10;
            p = DAT_8009f0ec;
            dx = x - (p->h->p.whole - p->box0);
            dy = yy - (p->y.p.whole + (p->box3 - p->box2));
            if ((unsigned short)dx < p->box1) {
                if (o->b69 || func_801215C0(o, dx, dy, p->subtype)) {
                    o->d8c = DAT_801152e8[o->wb0];
                    (*(unsigned char*)&o->waa) = 0;
                    o->b9e = 0;
                    o->step = 0;
                    o->state = 0;
                }
            }
        }
        {
            Fix16 *h = o->h;
            v = h->p.whole;
            if (o->animFrame & 1)
                h->p.whole = v - 2;
            else
                h->p.whole = v + 2;
        }
        v = o->h->p.whole;
        if (o->animFrame & 1)
            x = v - 8;
        else
            x = v + 8;
        yy = o->y.p.whole - 0x10;
        p = DAT_8009f0ec;
        dx = x - (p->h->p.whole - p->box0);
        dy = yy - (p->y.p.whole + (p->box3 - p->box2));
        if ((unsigned short)dx < p->box1) {
            s = columnProbeB(o, dx, dy, p->subtype);
            switch (s) {
            case 0:
                v = 1; (*(unsigned char*)&DAT_8009c330->w08) = v;
                {
                    Fix16 *h = o->h;
                    short n;
                    v = h->p.whole;
                    if (o->animFrame & 1)
                        n = v + 4;
                    else
                        n = v - 4;
                    h->p.whole = n;
                }
                o->velX = 0;
                o->velY = 0;
                o->wb2 = 0;
                o->velH = 0;
                o->velV = 0;
                (*(unsigned char*)&o->wac) = 1;
                playerSetFallLean(o);
                o->step = 2;
                o->state = 3;
                (*(unsigned char*)&o->waa) = 0;
                break;
            case 2:
                o->b9e = 5;
                o->step = 0x26;
                o->state = 0;
                break;
            }
        }
        break;
    }
    if (SPAD.s1fc & SPAD.s3c6) {
        {
            Fix16 *h; int f;
        ((unsigned char*)&GV2.bb0)[0] = 0;
        f = 1;
        o->b9c = f;
        o->b9e = 0;
        (*(unsigned char*)&o->waa) = 0;
            h = o->h;
            f = o->animFrame & 1;
            v = h->p.whole;
            if (f) h->p.whole = v + 0x10; else h->p.whole = v - 0x10;
        }
        if (GAME.area00_eventControl & 0x40) {
            if (PAD_PRESSED[0] & SPAD.s3c4)
                o->ba7 = 1;
        }
        o->step = 2;
        o->state = 0;
    }
    if (o->b69) {
        FUN_8001e5f4(0x1c, 0x7f);
        (*(unsigned char*)&DAT_8009c330->w08) = 0;
        o->ba7 = 0;
        (*(unsigned char*)&o->wac) = 0;
        o->b9c = 0;
        o->b9e = 0;
        (*(unsigned char*)&o->waa) = 0;
        o->d8c = DAT_801152e8[o->wb0];
        o->step = 3;
        o->state = 1;
    } else if (FUN_8003fd78(o, 0, 0)) {
        FUN_8001e5f4(0x1c, 0x7f);
        (*(unsigned char*)&DAT_8009c330->w08) = 0;
        o->ba7 = 0;
        (*(unsigned char*)&o->wac) = 0;
        o->b9c = 0;
        o->b9e = 0;
        (*(unsigned char*)&o->waa) = 0;
        o->d8c = DAT_801152e8[o->wb0];
        o->step = 0;
        o->state = 0;
    }
}
