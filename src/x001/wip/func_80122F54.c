#include "spad.h"
/* opusAA: 294 -> 281. `s4 = (short)s2` as a v0-pinned `sv_ = s2 << 16; s4 = sv_ >> 16` (290); before the
   func_8012767C call the args are pinned a0/a1 (a0 bumped) and set first, o->h via a v0-pinned pointer and
   h->p.whole read into a v0-pinned ushort before the 0xea store (281).
   Structural finding (not kept, 373): the game's BLKB puts x=TR(rcos) in s0 and y=TR(rsin) in s2 (`s0 = x; s2 = y`
   after `s1 = C`), BLKA puts x in s2 and y in s0 (`s2 = TR(rcos); s0 = TR(rsin)`); with that shape plus s1 pinned
   $17 and plain `s4 = (short)s2`, s1 lands in s1 and the s5 save goes away, but the `s2 & 0xfff` and
   `(s4 + 0x800) & 0xfff` temps are done in place (game: v1) and 5 instrs are missing; v1 pins / keep-alives on s2
   or s4 there all scored 369-503. */
#include "gv2.h"
/* score 294 (opusS, was 411): BLKA/BLKB use block-local temps for the two TR(rcos/rsin, s0) results (with pinned
   s-regs gcc set the rsin arg after the TR; temps give the game's "move a0,s0; sll; jal; sra" order), BLKB reads
   the old s2 for C before the rsin TR lands (game semantics) and now has the same shape as BLKA, so its tail
   cross-jumps into the shared BLKA tail as in the game; s1 no longer pinned (390 -> 298; gcc puts it in s5 and the
   TR temps in s1, so one extra s5 save); `s2 = x;` before the BLKA rcos NTR (294). Left: s1 var in s5 instead of s1
   (+ the s5 save), in-place temps where the game uses v1/v0: `(s4 + 0x800) & 0xfff` and `s2 & 0xfff` (ours reuse the
   dying s4/s2), `s4 = (short)s2` via v0, top-of-case a0/a1/v1 choices, case-1 sll/sra placement.
   Previous: o pinned s3 (param o_ copied to register asm("$19")), s0/s1/s2/s4 pinned s0/s1/s2/s4; the
   !animFrame ladders test s4 <= 0x800 first (BLKA branch first, as in the game); TR split into "x = f(); x <<= 3; x >>= 12".
   Tried 9 BLKA orderings (411-427); -fno-schedule-insns 396; unpinning s2/s4 382-436. Sibling of X009 func_8011E688. */
// FUNC 80122f54 2192 X001
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
    { int x = TR(rcos, s0); int y = TR(rsin, s0); \
    s1 -= (s4 + 0x800) & 0xfff; \
    s2 = x; s4 = s2 + NTR(rcos, s1); \
    s0 = y + NTR(rsin, s1); } \
}
#define BLKB(C) { \
    { int x = TR(rcos, s0); int y = TR(rsin, s0); s1 = C; s0 = x; \
    s4 = s0 + NTR(rcos, s1); \
    s0 = y + NTR(rsin, s1); } \
}

void func_80122F54(TObj *o_)
{
    register TObj *o asm("$19") = o_;
    TObj *g;
    int a;
    register int s0 asm("$16");
    int s1;
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
        if ((*(unsigned short*)&DAT_8009c330->type)) o->y.raw += 0x10000;
        (*(unsigned short*)&DAT_8009c330->type) = 1;
        {
            volatile unsigned short *k = DAT_8009d670;
            if (!(*k & 0x50)) {
                PlayerSetAnimIfChanged(o, 0xc);
            } else if (*k & 0x10) {
                PlayerSetAnimIfChanged(o, 0x2d);
                o->y.raw += -0x18000;
                a = o->h->p.whole;
                FUN_800408d8(o, (short)((o->animFrame & 1) ? a - 4 : a + 4), (short)(o->y.p.whole - 0x15));
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
            func_801244A0(o, g, 2, (g->d8c + 0x400) & 0xfff);
            break;
        case 2:
            func_801244A0(o, g, 1, g->d8c);
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
        g = DAT_8009f0ec;
        switch (g->w7a) {
        case 0:
        case 1:
            s2 = g->d8c;
            if (o->animFrame & 1) {
                o->h->p.whole -= 4;
                { register int sv_ asm("$2") = s2 << 16; s4 = sv_ >> 16; }
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
                    BLKB(s1 - (s2 & 0xfff));
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
