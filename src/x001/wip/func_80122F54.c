/* score 516: first draft. Structure/size match; game keeps o in s3 and calls rcos before rsin in the BLKB blocks (cross-jumped), regs s0/s1 swapped vs ours. Sibling of X009 wip func_8011E688. */
// FUNC 80122f54 2192 X001
/* whole function: includes the csv piece 801231F4 */
#include "TOBJ.H"
#include "raw7.h"
extern TObj *DAT_8009c330;
extern TObj *DAT_8009f0ec;
extern unsigned char DAT_801152e8[];
extern volatile unsigned short DAT_8009d670[];
extern unsigned short D_1F8001F8, D_1F8001FC, D_1F8003C6, D_1F8003C4;
extern unsigned char D_8009D2B0;
extern int D_8009C984;
extern void PlayerSetAnimIfChanged(TObj *, int);
extern int AnimAdvance(TObj *);
extern void SfxPlay2(int, int);
extern void SfxPlay3(int, int);
extern int rsin(int);
extern int rcos(int);
extern int FUN_800408d8(TObj *, int, int);
extern short TileCollideAt(TObj *, int, int);
extern void FUN_800ee428(TObj *);
extern void func_801244A0(TObj *, TObj *, int, int);
extern int func_8012767C(TObj *, TObj *);

#define TR(f, x) ((f(x) << 3) >> 12)
#define NTR(f, x) (-(f(x) << 3) >> 12)
#define BLKA { \
    s0 &= 0xfff; \
    s1 = 0x1000; \
    s0 = s1 - s0; \
    s2 = TR(rcos, s0); \
    s0 = TR(rsin, s0); \
    s1 -= (s4 + 0x800) & 0xfff; \
    s4 = s2 + NTR(rcos, s1); \
    s0 = s0 + NTR(rsin, s1); \
}
#define BLKB(C) { \
    s2 = TR(rsin, s0); \
    s0 = TR(rcos, s0); \
    s1 = C; \
    s4 = s0 + NTR(rcos, s1); \
    s0 = s2 + NTR(rsin, s1); \
}

void func_80122F54(TObj *o)
{
    TObj *g;
    int a, s0, s1, s2, s4;
    short v;

    switch (o->state) {
    case 0:
        PlayerSetAnimIfChanged(o, 0xa);
        o->d8c = 0;
        o->wb6 = 0;
        U16(DAT_8009c330, 2) = 0;
        U8(o, 0xaa) = 1;
        {
            int t = o->wb8;
            Fix16 *h;
            o->wb2 = 0;
            o->velH = 0;
            o->velV = 0;
            h = o->h;
            a = DAT_8009f0ec->h->p.whole + t;
            if (o->animFrame & 1) a += 8;
            else a -= 8;
            h->p.whole = a;
        }
        o->y.p.whole = DAT_8009f0ec->y.p.whole + o->wba + 8;
        SfxPlay3(4, 0x7f);
        o->state++;
    case 1:
        AnimAdvance(o);
        g = DAT_8009f0ec;
        switch (g->w7a) {
        case 0:
        case 1:
            a = g->d8c;
            if (a < 0x800) {
                a &= 0xfff;
                goto pos;
            }
            a &= 0xfff;
            goto neg;
        case 2:
            a = (g->d8c + 0x400) & 0xfff;
            if (a < 0x800) {
            pos:
                s0 = (unsigned int)(rsin(a) * (DAT_8009f0ec->box0 - 0x24)) >> 12;
            } else {
            neg:
                s0 = (unsigned int)-(rsin(a) * (DAT_8009f0ec->box0 - 0x24)) >> 12;
            }
            break;
        }
        if (U16(DAT_8009c330, 2)) o->y.raw += 0x10000;
        U16(DAT_8009c330, 2) = 1;
        {
            volatile unsigned short *k = DAT_8009d670;
            if (!(*k & 0x50)) {
                PlayerSetAnimIfChanged(o, 0xc);
            } else if (*k & 0x10) {
                PlayerSetAnimIfChanged(o, 0x2d);
                o->y.raw += -0x18000;
                a = o->h->p.whole;
                FUN_800408d8(o, (short)((o->animFrame & 1) ? a - 4 : a + 4), (short)(o->y.p.whole - 0x15));
                if ((D_1F8001F8 & 0xf) == 0) SfxPlay2(0x1d, 0);
            } else if (*k & 0x40) {
                PlayerSetAnimIfChanged(o, 0xc);
                if ((D_1F8001F8 & 0xf) == 0) SfxPlay2(0x1d, 0);
                o->y.raw += 0x18000;
            }
        }
        g = DAT_8009f0ec;
        switch (g->w7a) {
        case 1:
            func_801244A0(o, g, 2, (g->d8c + 0x400) & 0xfff);
            break;
        case 2:
            func_801244A0(o, g, 1, g->d8c);
            break;
        }
        if (o->b69 || (a = o->h->p.whole, TileCollideAt(o, (short)((o->animFrame & 1) ? a + 9 : a - 9), (short)(o->y.p.whole + 0x10)))) {
            o->d8c = DAT_801152e8[o->wb0];
            U16(DAT_8009c330, 2) = 0;
            U8(o, 0xaa) = 0;
            o->b9e = 0;
            o->step = 0;
            o->state = 0;
            break;
        }
        v = DAT_8009f0ec->y.p.whole - s0;
        if (v >= o->y.p.whole) o->y.p.whole = v;
        g = DAT_8009f0ec;
        switch (g->w7a) {
        case 0:
        case 1:
            s2 = g->d8c;
            if (o->animFrame & 1) {
                o->h->p.whole -= 4;
                s4 = (short)s2;
                if (s4 >= 0x800) {
                    o->d8c = ((s4 + 0x400) >> 4) & 0xff;
                    s0 = s4 + 0xc00;
                    s0 &= 0xfff;
                    s1 = 0x1000;
                    s0 = s1 - s0;
                    BLKB(s1 - (s2 & 0xfff));
                } else {
                    o->d8c = ((s4 + 0xc00) >> 4) & 0xff;
                    s0 = s4 + 0x400;
                    BLKA;
                }
            } else {
                o->h->p.whole += 4;
                s4 = (short)s2;
                if (s4 > 0x800) {
                    s0 = s4 + 0x400;
                    o->d8c = (s0 >> 4) & 0xff;
                    s0 &= 0xfff;
                    s1 = 0x1000;
                    s0 = s1 - s0;
                    BLKB(s1 - (s2 & 0xfff));
                } else {
                    s0 = s4 + 0xc00;
                    o->d8c = (s0 >> 4) & 0xff;
                    BLKA;
                }
            }
            break;
        case 2:
            s0 = (g->d8c + 0x400) & 0xfff;
            if (o->animFrame & 1) {
                o->h->p.whole -= 4;
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
                o->h->p.whole += 4;
                s4 = s0;
                if (s4 > 0x800) {
                    s0 = s4 + 0x400;
                    o->d8c = (s0 >> 4) & 0xff;
                    s0 &= 0xfff;
                    s1 = 0x1000;
                    s0 = s1 - s0;
                    BLKB(s1 - s4);
                } else {
                    s0 = s4 + 0xc00;
                    o->d8c = (s0 >> 4) & 0xff;
                    BLKA;
                }
            }
            break;
        }
        S16(o, 0xea) = o->y.p.whole + s0;
        S16(o, 0xe8) = o->h->p.whole + s4;
        if (func_8012767C(o, DAT_8009f0ec) == 0) {
            a = o->h->p.whole;
            if (o->animFrame & 1) o->h->p.whole = a + 4;
            else o->h->p.whole = a - 4;
            o->b9e = 0;
            o->velX = 0;
            o->velY = 0;
            o->wb2 = 0;
            o->velH = 0;
            o->velV = 0;
            U16(DAT_8009c330, 2) = 0;
            U8(o, 0xaa) = 0;
            U8(o, 0xac) = 1;
            FUN_800ee428(o);
            o->step = 2;
            o->state = 3;
            break;
        }
        g = DAT_8009f0ec;
        o->h->p.whole = g->h->p.whole + o->wb8 - s4;
        o->y.p.whole = g->y.p.whole + o->wba - s0;
        break;
    }
    if (D_1F8001FC & D_1F8003C6) {
        D_8009D2B0 = 0;
        U8(o, 0xaa) = 0;
        o->b9c = 1;
        o->b9e = 0;
        U16(DAT_8009c330, 2) = 0;
        a = o->h->p.whole;
        if (o->animFrame & 1) o->h->p.whole = a + 0x10;
        else o->h->p.whole = a - 0x10;
        if (D_8009C984 & 0x40) {
            if (DAT_8009d670[0] & D_1F8003C4) o->ba7 = 1;
        }
        o->step = 2;
        o->state = 0;
    }
}
