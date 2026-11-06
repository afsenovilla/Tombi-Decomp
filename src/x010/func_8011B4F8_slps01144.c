// VERSION SLPS_01144 x10 8011B4F8 2356
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
/* Matched (opusAE): o is the plain parameter (not a $19 copy, which let gcc drop the case-0 `move a0,s3`); BLKA/BLKB
   keep the raw rsin result and shift it after `s1 -= ...` with a0 pinned for the next rcos; the `& 0xfff` temps,
   the h -/+= 4 updates, the three D_800A0C7C loads (a0/v1/a0) and a few case-0/case-1 temps are register-pinned. */
#include "spad.h"
#include "gv2.h"
/* whole function: includes the csv piece 801231F4 */
#include "TOBJ.H"
#include "raw7.h"
extern TObj *D_8009DEE0;
extern TObj *D_800A0C7C;
extern unsigned char D_801145D4[];
extern volatile unsigned short D_8009F200[];
extern int D_8009E52C;
extern void PlayerSetAnimIfChanged(TObj *, int);
extern int AnimAdvance(TObj *);
extern void func_8001E388(int, int);
extern void func_8001E458(int, int);
extern int rsin(int);
extern int rcos(int);
extern int probeCollisionAtDepthA(TObj *, int, int);
extern short TileCollideAt(TObj *, int, int);
extern void playerSetFallLean(TObj *);
extern void func_8011C81C(TObj *, TObj *, int, int);
extern int func_8011D61C_slps01144(TObj *, TObj *);

#define TR(f, x) ((f(x) << 3) >> 12)
#define NTR(f, x) (-(f(x) << 3) >> 12)
#define BLKA { \
    s0 &= 0xfff; \
    s1 = 0x1000; \
    s0 = s1 - s0; \
    { int x = TR(rcos, s0); int r = rsin(s0); \
    { register int t asm("$3") = (s4 + 0x800) & 0xfff; s1 -= t; } \
    { register int a_ asm("$4") = s1; s0 = r << 3; s0 >>= 12; s2 = x; s4 = s2 + (-(rcos(a_) << 3) >> 12); } \
    s0 = s0 + NTR(rsin, s1); } \
}
#define BLKB(C) { \
    { int x = TR(rcos, s0); int r = rsin(s0); s1 = C; s0 = x; { register int a_ asm("$4") = s1; s2 = r << 3; s2 >>= 12; \
    s4 = s0 + (-(rcos(a_) << 3) >> 12); } \
    s0 = s2 + NTR(rsin, s1); } \
}

void func_8011B4F8_slps01144(TObj *o)
{
    TObj *g;
    register TObj *gA asm("$4");
    register TObj *gB asm("$4");
    register TObj *gC asm("$4");
    int a;
    register int s0 asm("$16");
    register int s1 asm("$17");
    register int s2 asm("$18");
    register int s4 asm("$20");
    short v;
    int pad_[4];

    if (0) func_8001E388((int)pad_, 0);
    switch (o->state) {
    case 0:
        PlayerSetAnimIfChanged(o, 0xa);
        o->d8c = 0;
        o->wb6 = 0;
        (*(unsigned short*)&D_8009DEE0->type) = 0;
        (*(unsigned char*)&o->waa) = 1;
        {
            register int t asm("$3") = o->wb8;
            register int sm asm("$4");
            Fix16 *h;
            o->wb2 = 0;
            o->velH = 0;
            o->velV = 0;
            h = o->h;
            sm = D_800A0C7C->h->p.whole + t;
            if (o->animFrame & 1) t = sm + 8;
            else t = sm - 8;
            h->p.whole = t;
        }
        o->y.p.whole = D_800A0C7C->y.p.whole + o->wba + 8;
        func_8001E458(4, 0x7f);
        o->state++;
    case 1:
        AnimAdvance(o);
        { register int aq asm("$4");
        gA = D_800A0C7C;
        switch (gA->w7a) {
        case 0:
            { register int hv asm("$2") = *(short*)&gA->d30;
            aq = gA->d30;
            if (hv < 0x800) {
                asm(""); aq &= 0xfff;
                goto pos;
            }
            asm(""); aq &= 0xfff;
            goto neg; }
        case 1:
            { register int hv asm("$2") = *(short*)&gA->d30;
            aq = gA->d30;
            if (hv < 0x800) {
                aq &= 0xfff;
                goto pos;
            }
            asm("#2"); aq &= 0xfff;
            goto neg; }
        case 2:
            { register int q asm("$2");
            if (gA->subtype == 3) q = gA->d30;
            else q = gA->d30 + 0x400;
            s2 = q & 0xfff; }
            if (s2 < 0x800) {
                aq = s2;
            pos:
                s0 = (unsigned int)(rsin(aq) * (D_800A0C7C->box0 - 0x24)) >> 12;
            } else {
                aq = s2;
            neg:
                { register unsigned int q asm("$2") = -(rsin(aq) * (D_800A0C7C->box0 - 0x24)); s0 = q >> 12; }
            }
            break;
        }
        }
        if ((*(short*)&D_8009DEE0->type)) o->y.raw += 0x10000;
        (*(unsigned short*)&D_8009DEE0->type) = 1;
        {
            volatile unsigned short *k = D_8009F200;
            if (!(*k & 0x5000)) {
                PlayerSetAnimIfChanged(o, 0xc);
            } else if (*k & 0x1000) {
                PlayerSetAnimIfChanged(o, 0x2d);
                o->y.raw += -0x18000;
                a = o->h->p.whole;
                probeCollisionAtDepthA(o, (short)((o->animFrame & 1) ? a - 4 : a + 4), (short)(o->y.p.whole - 0x15));
                if ((SPAD.s1f8 & 0xf) == 0) func_8001E388(0x1d, 0);
            } else if (*k & 0x4000) {
                PlayerSetAnimIfChanged(o, 0xc);
                if ((SPAD.s1f8 & 0xf) == 0) func_8001E388(0x1d, 0);
                o->y.raw += 0x18000;
            }
        }
        g = D_800A0C7C;
        if (g->subtype != 3) {
        switch (g->w7a) {
        case 1:
            func_8011C81C(o, g, 2, (g->d30 + 0x400) & 0xfff);
            break;
        case 2:
            func_8011C81C(o, g, 1, g->d30);
            break;
        }
        }
        if (o->b69 || (a = o->h->p.whole, TileCollideAt(o, (short)((o->animFrame & 1) ? a + 9 : a - 9), (short)(o->y.p.whole + 0x10)))) {
            o->d8c = D_801145D4[o->wb0];
            (*(unsigned short*)&D_8009DEE0->type) = 0;
            (*(unsigned char*)&o->waa) = 0;
            o->b9e = 0;
            o->step = 0;
            o->state = 0;
            break;
        }
        v = D_800A0C7C->y.p.whole - s0;
        if (v >= o->y.p.whole) o->y.p.whole = v;
        gB = D_800A0C7C;
        switch (gB->w7a) {
        case 0:
        case 1:
            s2 = gB->d30;
            if (o->animFrame & 1) {
                { register Fix16 *h_ asm("$3") = o->h; register unsigned short t_ asm("$2") = h_->p.whole - 4; h_->p.whole = t_; }
                { register int sv_ asm("$2") = s2 << 16; s4 = sv_ >> 16; }
                if (s4 >= 0x800) {
                    o->d8c = ((s4 + 0x400) >> 4) & 0xff;
                    s0 = s4 + 0xc00;
                    s0 &= 0xfff;
                    s1 = 0x1000;
                    s0 = s1 - s0;
                    BLKB(s1 - ({ register int t asm("$3") = s2 & 0xfff; t; }));
                } else {
                    o->d8c = ((s4 + 0xc00) >> 4) & 0xff;
                    s0 = s4 + 0x400;
                    BLKA;
                }
            } else {
                { register Fix16 *h_ asm("$3") = o->h; register unsigned short t_ asm("$2") = h_->p.whole + 4; h_->p.whole = t_; }
                { register int sv_ asm("$2") = s2 << 16; s4 = sv_ >> 16; }
                if (s4 <= 0x800) {
                    s0 = s4 + 0xc00;
                    o->d8c = (s0 >> 4) & 0xff;
                    BLKA;
                } else {
                    s0 = s4 + 0x400;
                    o->d8c = (s0 >> 4) & 0xff;
                    s0 &= 0xfff;
                    s1 = 0x1000;
                    s0 = s1 - s0;
                    BLKB(s1 - ({ register int t asm("$3") = s2 & 0xfff; t; }));
                }
            }
            break;
        case 2:
            { register int q asm("$2");
            if (gB->subtype == 3) q = gB->d30;
            else q = gB->d30 + 0x400;
            s2 = q & 0xfff; }
            s4 = s2;
            if (o->animFrame & 1) {
                { register Fix16 *h_ asm("$2") = o->h; register unsigned short t_ asm("$3") = h_->p.whole - 4; h_->p.whole = t_; }
                if (s4 >= 0x800) {
                    o->d8c = ((s4 + 0x400) >> 4) & 0xff;
                    s0 = s4 + 0xc00;
                    s0 &= 0xfff;
                    s1 = 0x1000;
                    s0 = s1 - s0;
                    BLKB(s1 - s2);
                } else {
                    o->d8c = ((s4 + 0xc00) >> 4) & 0xff;
                    s0 = s4 + 0x400;
                    BLKA;
                }
            } else {
                { register Fix16 *h_ asm("$2") = o->h; register unsigned short t_ asm("$3") = h_->p.whole + 4; h_->p.whole = t_; }
                if (s4 <= 0x800) {
                    s0 = s4 + 0xc00;
                    o->d8c = (s0 >> 4) & 0xff;
                    BLKA;
                } else {
                    s0 = s4 + 0x400;
                    o->d8c = (s0 >> 4) & 0xff;
                    s0 &= 0xfff;
                    s1 = 0x1000;
                    s0 = s1 - s0;
                    BLKB(s1 - s2);
                }
            }
            break;
        }
        { register TObj *a0_ asm("$4") = o; register TObj *a1_ asm("$5"); register Fix16 *h asm("$2");
        asm("" : "=r"(a0_) : "0"(a0_)); a1_ = D_800A0C7C; h = o->h;
        { register unsigned short hw asm("$2") = h->p.whole;
        S16(o, 0xda) = o->y.p.whole + s0;
        S16(o, 0xd8) = hw + s4; }
        a = func_8011D61C_slps01144(a0_, a1_); }
        if (a == 0) {
            a = o->h->p.whole;
            if (o->animFrame & 1) o->h->p.whole = a + 4;
            else o->h->p.whole = a - 4;
            o->b9e = 0;
            o->velX = 0;
            o->velY = 0;
            o->wb2 = 0;
            o->velH = 0;
            o->velV = 0;
            (*(unsigned short*)&D_8009DEE0->type) = 0;
            (*(unsigned char*)&o->waa) = 0;
            (*(unsigned char*)&o->wac) = 1;
            { int fa = 16;
              o->d84 = 0; if (o->animFrame & 1) fa = 0xf0; o->timer = 10; o->d88 = fa; o->d8c = 0; }
            o->step = 2;
            o->state = 3;
            break;
        }
        gC = D_800A0C7C;
        o->h->p.whole = gC->h->p.whole + o->wb8 - s4;
        o->y.p.whole = gC->y.p.whole + o->wba - s0;
        break;
    }
    if (SPAD.s1fc & SPAD.s3c6) {
        (*(unsigned char*)&o->waa) = 0;
        o->b9c = 1;
        o->b9e = 0;
        (*(unsigned short*)&D_8009DEE0->type) = 0;
        a = o->h->p.whole;
        if (o->animFrame & 1) o->h->p.whole = a + 0x10;
        else o->h->p.whole = a - 0x10;
        if (D_8009E52C & 0x40) {
            if (D_8009F200[0] & SPAD.s3c4) o->ba7 = 1;
        }
        o->step = 2;
        o->state = 0;
    }
}
#endif
