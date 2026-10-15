// FUNC 80122410 2220 X001
// MATCHING 80122410 2220
#include "spad.h"
#include "gv6.h"
/* Debt: the __asm__ volatile("") barrier before the final anim set (the empty do/while(0) an earlier agent used
   pinned the la ahead of the AnimJump arg moves). Matched by making g single-set in case 1 (second read into g2,
   case 4 pointer into g4: both D_8009C330/D_8009F0EC loads get the birthing boost and keep source order) and by
   assigning the case 4 orbit pointer through a brace-block macro so g4 spans two blocks (global-alloc gives it a2
   after the local-alloc'd division constant takes a1). */
#include "TOBJ.H"
typedef struct {
    unsigned char b0, b1;
    short w02;
    unsigned char b4, b5, b6, b7, b8, b9;
    char pa[0xc - 0xa];
    short w0c, w0e, w10, w12;
    char p14[0x28 - 0x14];
    unsigned short w28, w2a;
    char p2c[0x2e - 0x2c];
    unsigned short w2e;
} P;
typedef struct L { unsigned char active; char p[0x94 - 1]; struct L *next; } L;
extern P *D_8009C330;
extern TObj *D_8009F0EC;
extern void FUN_80010d68();
extern void FUN_80010dac();
extern void SfxPlay3(int, int);
extern void SfxPlay2(int, int);
extern void AnimJump(TObj *, int);
extern int AnimAdvance(TObj *);
extern int rcos(int);
extern int rsin(int);
extern void PlayerSetAnimIfChanged(TObj *, int);
extern TObj *playerProbeGrab(TObj *, int);
extern void playerSetFallLean(TObj *);
extern void playerTryAttack(TObj *, int);
extern void playerClingJumpRelease(TObj *);

static __inline__ TObj *orbit2(TObj *o, int a)
{
    int c, s;
    TObj *g;
    TObj *g2;
    TObj *g4;
    a = (a + 0x400) & 0xfff;
    c = (rcos(a) * o->wba) >> 12;
    s = rsin(a) * o->wba;
    g = D_8009F0EC;
    o->h->p.whole = o->wb8 + (g->h->p.whole + c) + g->d30;
    o->y.p.whole = g->y.p.whole + (s >> 12);
    return g;
}
#define orbit(o) orbit2(o, D_8009F0EC->d8c)
#define ORBIT(o, gv) { int c_, s_, a_; \
    a_ = (D_8009F0EC->d8c + 0x400) & 0xfff; \
    c_ = (rcos(a_) * (o)->wba) >> 12; \
    s_ = rsin(a_) * (o)->wba; \
    gv = D_8009F0EC; \
    (o)->h->p.whole = (o)->wb8 + (gv->h->p.whole + c_) + gv->d30; \
    (o)->y.p.whole = gv->y.p.whole + (s_ >> 12); }

void playerStepClingRotating(TObj *o)
{
    short t[12] = { 1, 1, 2, 2, 3, 3, 3, 3, 2, 2, 1, 1 };
    P *p;
    TObj *g;
    TObj *g2;
    TObj *g4;
    TObj *r;
    unsigned short *k;

    D_8009C330->b7 = o->animFrame;
    switch (o->state) {
    case 0:
        SfxPlay3(4, 0x7f);
        o->state++;
    case 1:
        p = D_8009C330;
        g = D_8009F0EC;
        o->wb6 = -0x420;
        p->w0e = 0;
        p->w02 = 0x10;
        o->wb2 = g->box1;
        o->animFrame &= 1;
        p->b6 = g->active;
        o->ba4 = 0;
        D_8009C330->b9 = 0;
        {
        P *q = D_8009C330;
        q->w2e = 0xffff;
        q->w28 = 0xffff;
        q->w2a = 0xffff;
        q->w0c = 0;
        }
        ((unsigned char *)&o->wa8)[1] = 1;
        g2 = D_8009F0EC;
        o->h->p.whole = o->wb8 + (g2->h->p.whole + g2->d30);
        o->y.p.whole = g2->y.p.whole + o->wba;
        o->velH = g2->h->p.whole - o->h->p.whole;
        o->velV = g2->y.p.whole - o->y.p.whole;
        o->wb0 = 0;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        o->velX = o->wb8;
        {
        P *r2 = D_8009C330;
        r2->w10 = g2->h->p.whole + o->wb8;
        r2->w12 = g2->y.p.whole + o->wba;
        }
        o->anim = (void *)FUN_80010d68;
        AnimJump(o, 0);
        o->timer = 10;
        o->state++;
    case 2:
        orbit(o);
        if (--o->timer == 0) o->state++;
        playerClingJumpRelease(o);
        break;
    case 3:
        AnimAdvance(o);
        k = &GV6.s70;
        if (!(*k & 0x50)) {
            switch (*(unsigned short *)o->anim) {
            case 0x75:
            case 0x149:
            case 0x14a:
            case 0x14b:
                break;
            default:
                o->anim = (void *)FUN_80010d68;
                AnimJump(o, 0);
                {
                    P *q = D_8009C330;
                    q->w2e = 0xffff;
                    q->w28 = 0xffff;
                    q->w2a = 0xffff;
                }
                break;
            }
            o->velY = 0;
            if (o->animFrame & 1) {
                if (SPAD.s1fc & 0x80) {
                    o->anim = (void *)FUN_80010dac;
                    AnimJump(o, 0);
                    D_8009C330->b4 = 1;
                    D_8009F0EC->b69 = 4;
                    SfxPlay2(0x1e, 8);
                    D_8009C330->w10 = -((D_8009F0EC->box1 << 8) / 24);
                    o->state++;
                }
            } else {
                if (SPAD.s1fc & 0x20) {
                    o->anim = (void *)FUN_80010dac;
                    AnimJump(o, 0);
                    D_8009C330->b4 = 1;
                    D_8009F0EC->b69 = 4;
                    SfxPlay2(0x1e, 8);
                    D_8009C330->w10 = (D_8009F0EC->box1 << 8) / 24;
                    o->state++;
                }
            }
        } else {
            if (!(SPAD.s1f8 & 0xf)) SfxPlay2(0x1d, 0);
            if (*k & 0x10) {
                PlayerSetAnimIfChanged(o, 0x25);
                if (D_8009F0EC->y.p.whole - D_8009F0EC->d38 - 0x10 < o->y.p.whole) o->y.raw += -0x18000;
            } else {
                PlayerSetAnimIfChanged(o, 0x4a);
                o->y.raw += 0x28000;
            }
        }
        o->b9e = 0;
        r = playerProbeGrab(o, 0);
        if (r == 0) {
            L *q, *n;
            ((unsigned char *)&o->wa8)[1] = 0;
            n = ((L *)D_8009F0EC)->next;
            if (n) {
                q = n;
                goto chk;
            loop:
                q->active = 3;
                q = q->next;
            chk:
                if (q->next) goto loop;
                q->active = 1;
            }
            D_8009F0EC = r;
            o->b9c = 2;
            o->velH = 0;
            o->velV = 0;
            o->velX = 0;
            o->velY = 0;
            playerSetFallLean(o);
            D_8009C330->b4 = 0;
            o->step = 2;
            o->state = 3;
            break;
        }
        D_8009F0EC = r;
        orbit(o);
        o->d88 = 0;
        playerTryAttack(o, 2);
        playerClingJumpRelease(o);
        break;
    case 4:
        AnimAdvance(o);
        ORBIT(o, g4)
        o->wb8 += (D_8009C330->w10 * t[o->d88 % 12]) >> 8;
        if (++o->d88 >= 12) {
            o->animFrame ^= 1;
            if (o->animFrame & 1) o->wb8 = o->velX + o->wb2;
            else o->wb8 = o->velX - o->wb2;
            orbit2(o, g4->d8c);
            __asm__ volatile("");
            o->anim = (void *)FUN_80010d68;
            AnimJump(o, 0);
            o->state = 1;
            o->velY = 0;
            o->d84 = 0;
            o->d88 = 0;
            D_8009C330->b4 = 0;
        }
        break;
    }
}
