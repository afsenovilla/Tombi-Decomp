// FUNC 80122410 2220 X001
// MATCHING 80122410 2220
// VERSION SLPS_01144 x10 8011AA10 2112
#if defined(VERSION_SLPS_01144)
#include "spad.h"
#include "gv6.h"
#include "TOBJ.H"
typedef struct {
    char pad0[2];
    short w02;
    unsigned char b4;
    char pad5;
    unsigned char b6;
    unsigned char b7;
    unsigned char b8;
    unsigned char b9;
    char pada[2];
    short w0c;
    short w0e;
    short w10;
    short w12;
    char pad14[0x14];
    unsigned short w28;
    unsigned short w2a;
    char pad2c[2];
    unsigned short w2e;
} G330;
extern G330 *PLAYER_EXT;
extern TObj *GRABBED_OBJECT;
extern void PLAYER_CROUCH_ANIM();
extern void FUN_80010dac();
extern void AnimJump(TObj *, int);
extern int AnimAdvance(TObj *);
extern void SfxPlay2(int, int);
extern void SfxPlay3(int, int);
extern int rcos(int);
extern int rsin(int);
extern TObj *playerProbeGrab(TObj *, int);
extern void PlayerSetAnimIfChanged(TObj *, int);
extern void playerSetFallLean(TObj *);
extern void playerTryAttack(TObj *, int);
extern void playerClingJumpRelease(TObj *);

#define ORBIT(o) { \
    int a = (GRABBED_OBJECT->d8c + 0x400) & 0xfff; \
    int x = rcos(a) * o->wba >> 12; \
    int y = rsin(a) * o->wba; \
    o->h->p.whole = o->wb8 + (GRABBED_OBJECT->h->p.whole + x) + GRABBED_OBJECT->d30; \
    o->y.p.whole = GRABBED_OBJECT->y.p.whole + (y >> 12); \
}

void func_8011AA10(TObj *o)
{
    int v;
    short tbl[12] = { 1, 1, 2, 2, 3, 3, 3, 3, 2, 2, 1, 1 };
    unsigned short *pad;
    TObj *e;

    PLAYER_EXT->b7 = o->animFrame;
    switch (o->state) {
    case 0:
        SfxPlay3(4, 0x7f);
        o->state++;
    case 1:
        o->wb6 = -0x420;
        PLAYER_EXT->w0e = 0;
        PLAYER_EXT->w02 = 0x10;
        o->wb2 = GRABBED_OBJECT->box1;
        o->animFrame &= 1;
        PLAYER_EXT->b6 = GRABBED_OBJECT->active;
        PLAYER_EXT->b9 = 0;
        PLAYER_EXT->w2e = 0xffff;
        PLAYER_EXT->w28 = 0xffff;
        PLAYER_EXT->w2a = 0xffff;
        PLAYER_EXT->w0c = 0;
        ((unsigned char *)&o->wa8)[1] = 1;
        o->h->p.whole = o->wb8 + (GRABBED_OBJECT->h->p.whole + GRABBED_OBJECT->d30);
        o->y.p.whole = GRABBED_OBJECT->y.p.whole + o->wba;
        o->velH = GRABBED_OBJECT->h->p.whole - o->h->p.whole;
        o->velV = GRABBED_OBJECT->y.p.whole - o->y.p.whole;
        o->wb0 = 0;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        o->velX = o->wb8;
        PLAYER_EXT->w10 = GRABBED_OBJECT->h->p.whole + o->wb8;
        PLAYER_EXT->w12 = GRABBED_OBJECT->y.p.whole + o->wba;
        o->anim = (void *)PLAYER_CROUCH_ANIM;
        AnimJump(o, 0);
        o->timer = 10;
        o->state++;
    case 2:
        ORBIT(o);
        if (--o->timer == 0) o->state++;
        playerClingJumpRelease(o);
        break;
    case 3:
        AnimAdvance(o);
        pad = &GV6.s70;
        if (!(*pad & 0x5000)) {
            o->anim = (void *)PLAYER_CROUCH_ANIM;
            AnimJump(o, 0);
            PLAYER_EXT->w2e = 0xffff;
            PLAYER_EXT->w28 = 0xffff;
            PLAYER_EXT->w2a = 0xffff;
            o->velY = 0;
            if (o->animFrame & 1) {
                if (SPAD.s1fc & 0x8000) {
                    PLAYER_EXT->b4 = 1;
                    GRABBED_OBJECT->b69 = 4;
                    SfxPlay2(0x1e, 8);
                    PLAYER_EXT->w10 = -((GRABBED_OBJECT->box1 << 8) / 24);
                    o->state++;
                }
            } else {
                if (SPAD.s1fc & 0x2000) {
                    PLAYER_EXT->b4 = 1;
                    GRABBED_OBJECT->b69 = 4;
                    SfxPlay2(0x1e, 8);
                    PLAYER_EXT->w10 = (GRABBED_OBJECT->box1 << 8) / 24;
                    o->state++;
                }
            }
                } else {
            if ((SPAD.s1f8 & 0xf) == 0) SfxPlay2(0x1d, 0);
            PlayerSetAnimIfChanged(o, 0x25);
            if (*pad & 0x1000) {
                if (GRABBED_OBJECT->y.p.whole - GRABBED_OBJECT->d38 - 0x10 < o->y.p.whole)
                    o->y.raw += -0x18000;
            } else {
                o->y.raw += 0x28000;
            }
        }
        o->b9e = 0;
        e = playerProbeGrab(o, 0);
        if (e == 0) {
            TObj *q;
            ((unsigned char *)&o->wa8)[1] = 0;
            if (GRABBED_OBJECT->d94) {
                q = (TObj *)GRABBED_OBJECT->d94;
                goto test;
            body:
                q->active = 3;
                q = (TObj *)q->d94;
            test:
                if (q->d94) goto body;
                q->active = 1;
            }
            GRABBED_OBJECT = e;
            PLAYER_EXT->b4 = 0;
            o->b9c = 2;
            o->velH = 0;
            o->velV = 0;
            o->velX = 0;
            o->velY = 0;
            v = 0x10;
            *(int *)((char *)o + 0x84) = 0;
            if (*(unsigned short *)((char *)o + 0x2e) & 1) v = 0xf0;
            *(short *)((char *)o + 0x20) = 10;
            *(int *)((char *)o + 0x88) = v;
            *(int *)((char *)o + 0x8c) = 0;
            o->step = 2;
            o->state = 3;
            break;
        }
        GRABBED_OBJECT = e;
        ORBIT(o);
        o->d88 = 0;
        playerTryAttack(o, 2);
        playerClingJumpRelease(o);
        break;
    case 4:
        AnimAdvance(o);
        ORBIT(o);
        o->wb8 += (PLAYER_EXT->w10 * tbl[o->d88 % 12]) >> 8;
        if (++o->d88 < 12) break;
        o->animFrame ^= 1;
        if (o->animFrame & 1) {
            o->wb8 = o->velX + o->wb2;
            ORBIT(o);
        } else {
            o->wb8 = o->velX - o->wb2;
            ORBIT(o);
        }
        o->anim = (void *)PLAYER_CROUCH_ANIM;
        AnimJump(o, 0);
        o->velY = 0;
        o->d84 = 0;
        o->d88 = 0;
        PLAYER_EXT->b4 = 0;
        o->state = 1;
        break;
    }
}
#else
#include "spad.h"
#include "gv6.h"
/* Debt: the __asm__ volatile("") barrier before the final anim set (the empty do/while(0) an earlier agent used
   pinned the la ahead of the AnimJump arg moves). Matched by making g single-set in case 1 (second read into g2,
   case 4 pointer into g4: both PLAYER_EXT/GRABBED_OBJECT loads get the birthing boost and keep source order) and by
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
extern P *PLAYER_EXT;
extern TObj *GRABBED_OBJECT;
extern void PLAYER_CROUCH_ANIM();
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
    g = GRABBED_OBJECT;
    o->h->p.whole = o->wb8 + (g->h->p.whole + c) + g->d30;
    o->y.p.whole = g->y.p.whole + (s >> 12);
    return g;
}
#define orbit(o) orbit2(o, GRABBED_OBJECT->d8c)
#define ORBIT(o, gv) { int c_, s_, a_; \
    a_ = (GRABBED_OBJECT->d8c + 0x400) & 0xfff; \
    c_ = (rcos(a_) * (o)->wba) >> 12; \
    s_ = rsin(a_) * (o)->wba; \
    gv = GRABBED_OBJECT; \
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

    PLAYER_EXT->b7 = o->animFrame;
    switch (o->state) {
    case 0:
        SfxPlay3(4, 0x7f);
        o->state++;
    case 1:
        p = PLAYER_EXT;
        g = GRABBED_OBJECT;
        o->wb6 = -0x420;
        p->w0e = 0;
        p->w02 = 0x10;
        o->wb2 = g->box1;
        o->animFrame &= 1;
        p->b6 = g->active;
        o->ba4 = 0;
        PLAYER_EXT->b9 = 0;
        {
        P *q = PLAYER_EXT;
        q->w2e = 0xffff;
        q->w28 = 0xffff;
        q->w2a = 0xffff;
        q->w0c = 0;
        }
        ((unsigned char *)&o->wa8)[1] = 1;
        g2 = GRABBED_OBJECT;
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
        P *r2 = PLAYER_EXT;
        r2->w10 = g2->h->p.whole + o->wb8;
        r2->w12 = g2->y.p.whole + o->wba;
        }
        o->anim = (void *)PLAYER_CROUCH_ANIM;
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
                o->anim = (void *)PLAYER_CROUCH_ANIM;
                AnimJump(o, 0);
                {
                    P *q = PLAYER_EXT;
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
                    PLAYER_EXT->b4 = 1;
                    GRABBED_OBJECT->b69 = 4;
                    SfxPlay2(0x1e, 8);
                    PLAYER_EXT->w10 = -((GRABBED_OBJECT->box1 << 8) / 24);
                    o->state++;
                }
            } else {
                if (SPAD.s1fc & 0x20) {
                    o->anim = (void *)FUN_80010dac;
                    AnimJump(o, 0);
                    PLAYER_EXT->b4 = 1;
                    GRABBED_OBJECT->b69 = 4;
                    SfxPlay2(0x1e, 8);
                    PLAYER_EXT->w10 = (GRABBED_OBJECT->box1 << 8) / 24;
                    o->state++;
                }
            }
        } else {
            if (!(SPAD.s1f8 & 0xf)) SfxPlay2(0x1d, 0);
            if (*k & 0x10) {
                PlayerSetAnimIfChanged(o, 0x25);
                if (GRABBED_OBJECT->y.p.whole - GRABBED_OBJECT->d38 - 0x10 < o->y.p.whole) o->y.raw += -0x18000;
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
            n = ((L *)GRABBED_OBJECT)->next;
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
            GRABBED_OBJECT = r;
            o->b9c = 2;
            o->velH = 0;
            o->velV = 0;
            o->velX = 0;
            o->velY = 0;
            playerSetFallLean(o);
            PLAYER_EXT->b4 = 0;
            o->step = 2;
            o->state = 3;
            break;
        }
        GRABBED_OBJECT = r;
        orbit(o);
        o->d88 = 0;
        playerTryAttack(o, 2);
        playerClingJumpRelease(o);
        break;
    case 4:
        AnimAdvance(o);
        ORBIT(o, g4)
        o->wb8 += (PLAYER_EXT->w10 * t[o->d88 % 12]) >> 8;
        if (++o->d88 >= 12) {
            o->animFrame ^= 1;
            if (o->animFrame & 1) o->wb8 = o->velX + o->wb2;
            else o->wb8 = o->velX - o->wb2;
            orbit2(o, g4->d8c);
            __asm__ volatile("");
            o->anim = (void *)PLAYER_CROUCH_ANIM;
            AnimJump(o, 0);
            o->state = 1;
            o->velY = 0;
            o->d84 = 0;
            o->d88 = 0;
            PLAYER_EXT->b4 = 0;
        }
        break;
    }
}
#endif
