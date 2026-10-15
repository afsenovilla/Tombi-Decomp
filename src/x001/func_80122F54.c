// FUNC 80122f54 2192 X001
// MATCHING 80122f54 2192
/* Matched (opusAE): o is the plain parameter (not a $19 copy, which let gcc drop the case-0 `move a0,s3`); BLKA/BLKB
   keep the raw rsin result and shift it after `s1 -= ...` with a0 pinned for the next rcos; the `& 0xfff` temps,
   the h -/+= 4 updates, the three DAT_8009f0ec loads (a0/v1/a0) and a few case-0/case-1 temps are register-pinned. */
#include "spad.h"
#include "gv2.h"
/* whole function: includes the csv piece 801231F4 */
#include "TOBJ.H"
#include "raw7.h"
extern TObj *DAT_8009c330;
extern TObj *DAT_8009f0ec;
extern unsigned char DAT_801152e8[];
extern volatile unsigned short DAT_8009d670[];
extern int D_8009C984;
extern void PlayerSetAnimIfChanged(TObj *, int);
extern int AnimAdvance(TObj *);
extern void SfxPlay2(int, int);
extern void SfxPlay3(int, int);
extern int rsin(int);
extern int rcos(int);
extern int probeCollisionAtDepthA(TObj *, int, int);
extern short TileCollideAt(TObj *, int, int);
extern void playerSetFallLean(TObj *);
extern void collidePlayerWithTiltedBar(TObj *, TObj *, int, int);
extern int func_8012767C(TObj *, TObj *);

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

void func_80122F54(TObj *o)
{
    TObj *g;
    register TObj *gA asm("$4");
    register TObj *gB asm("$3");
    register TObj *gC asm("$4");
    int a;
    register int s0 asm("$16");
    register int s1 asm("$17");
    register int s2 asm("$18");
    register int s4 asm("$20");
    short v;

    switch (o->state) {
    case 0:
        PlayerSetAnimIfChanged(o, 0xa);
        o->d8c = 0;
        o->wb6 = 0;
        (*(unsigned short*)&DAT_8009c330->type) = 0;
        (*(unsigned char*)&o->waa) = 1;
        {
            register int t asm("$3") = o->wb8;
            register int sm asm("$4");
            Fix16 *h;
            o->wb2 = 0;
            o->velH = 0;
            o->velV = 0;
            h = o->h;
            sm = DAT_8009f0ec->h->p.whole + t;
            if (o->animFrame & 1) t = sm + 8;
            else t = sm - 8;
            h->p.whole = t;
        }
        o->y.p.whole = DAT_8009f0ec->y.p.whole + o->wba + 8;
        SfxPlay3(4, 0x7f);
        o->state++;
    case 1:
        AnimAdvance(o);
        { register int aq asm("$4");
        gA = DAT_8009f0ec;
        switch (gA->w7a) {
        case 0:
        case 1:
            aq = gA->d8c;
            if (aq < 0x800) {
                aq &= 0xfff;
                goto pos;
            }
            aq &= 0xfff;
            goto neg;
        case 2:
            { register int q asm("$2") = gA->d8c + 0x400; aq = q & 0xfff; }
            if (aq < 0x800) {
            pos:
                s0 = (unsigned int)(rsin(aq) * (DAT_8009f0ec->box0 - 0x24)) >> 12;
            } else {
            neg:
                { register unsigned int q asm("$2") = -(rsin(aq) * (DAT_8009f0ec->box0 - 0x24)); s0 = q >> 12; }
            }
            break;
        }
        }
        if ((*(short*)&DAT_8009c330->type)) o->y.raw += 0x10000;
        (*(unsigned short*)&DAT_8009c330->type) = 1;
        {
            volatile unsigned short *k = DAT_8009d670;
            if (!(*k & 0x50)) {
                PlayerSetAnimIfChanged(o, 0xc);
            } else if (*k & 0x10) {
                PlayerSetAnimIfChanged(o, 0x2d);
                o->y.raw += -0x18000;
                a = o->h->p.whole;
                probeCollisionAtDepthA(o, (short)((o->animFrame & 1) ? a - 4 : a + 4), (short)(o->y.p.whole - 0x15));
                if ((SPAD.s1f8 & 0xf) == 0) SfxPlay2(0x1d, 0);
            } else if (*k & 0x40) {
                PlayerSetAnimIfChanged(o, 0xc);
                if ((SPAD.s1f8 & 0xf) == 0) SfxPlay2(0x1d, 0);
                o->y.raw += 0x18000;
            }
        }
        g = DAT_8009f0ec;
        switch (g->w7a) {
        case 1:
            collidePlayerWithTiltedBar(o, g, 2, (g->d8c + 0x400) & 0xfff);
            break;
        case 2:
            collidePlayerWithTiltedBar(o, g, 1, g->d8c);
            break;
        }
        if (o->b69 || (a = o->h->p.whole, TileCollideAt(o, (short)((o->animFrame & 1) ? a + 9 : a - 9), (short)(o->y.p.whole + 0x10)))) {
            o->d8c = DAT_801152e8[o->wb0];
            (*(unsigned short*)&DAT_8009c330->type) = 0;
            (*(unsigned char*)&o->waa) = 0;
            o->b9e = 0;
            o->step = 0;
            o->state = 0;
            break;
        }
        v = DAT_8009f0ec->y.p.whole - s0;
        if (v >= o->y.p.whole) o->y.p.whole = v;
        gB = DAT_8009f0ec;
        switch (gB->w7a) {
        case 0:
        case 1:
            s2 = gB->d8c;
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
            { register int t2 asm("$2") = gB->d8c + 0x400; s0 = t2 & 0xfff; }
            if (o->animFrame & 1) {
                { register Fix16 *h_ asm("$2") = o->h; register unsigned short t_ asm("$3") = h_->p.whole - 4; h_->p.whole = t_; }
                s4 = s0;
                if (s4 >= 0x800) {
                    o->d8c = ((s4 + 0x400) >> 4) & 0xff;
                    s0 = s4 + 0xc00;
                    s0 &= 0xfff;
                    s1 = 0x1000;
                    s0 = s1 - s0;
                    BLKB(s1 - s4);
                } else {
                    o->d8c = ((s4 + 0xc00) >> 4) & 0xff;
                    s0 = s4 + 0x400;
                    BLKA;
                }
            } else {
                { register Fix16 *h_ asm("$2") = o->h; register unsigned short t_ asm("$3") = h_->p.whole + 4; h_->p.whole = t_; }
                s4 = s0;
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
                    BLKB(s1 - s4);
                }
            }
            break;
        }
        { register TObj *a0_ asm("$4") = o; register TObj *a1_ asm("$5"); register Fix16 *h asm("$2");
        asm("" : "=r"(a0_) : "0"(a0_)); a1_ = DAT_8009f0ec; h = o->h;
        { register unsigned short hw asm("$2") = h->p.whole;
        S16(o, 0xea) = o->y.p.whole + s0;
        S16(o, 0xe8) = hw + s4; }
        a = func_8012767C(a0_, a1_); }
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
            (*(unsigned short*)&DAT_8009c330->type) = 0;
            (*(unsigned char*)&o->waa) = 0;
            (*(unsigned char*)&o->wac) = 1;
            playerSetFallLean(o);
            o->step = 2;
            o->state = 3;
            break;
        }
        gC = DAT_8009f0ec;
        o->h->p.whole = gC->h->p.whole + o->wb8 - s4;
        o->y.p.whole = gC->y.p.whole + o->wba - s0;
        break;
    }
    if (SPAD.s1fc & SPAD.s3c6) {
        GV2.bb0 = 0;
        (*(unsigned char*)&o->waa) = 0;
        o->b9c = 1;
        o->b9e = 0;
        (*(unsigned short*)&DAT_8009c330->type) = 0;
        a = o->h->p.whole;
        if (o->animFrame & 1) o->h->p.whole = a + 0x10;
        else o->h->p.whole = a - 0x10;
        if (D_8009C984 & 0x40) {
            if (DAT_8009d670[0] & SPAD.s3c4) o->ba7 = 1;
        }
        o->step = 2;
        o->state = 0;
    }
}
