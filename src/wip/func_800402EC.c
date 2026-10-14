// FUNC 800402ec 880 MAIN0
// opusV: 82 -> 50 (52 + itbrute sx launder after r). b and xm both pinned to $9 (b dies at the divide, xm reuses t1 like the game, so (short)b is recomputed in v1), x & 7 computed in a $2-pinned temp tt then copied to xm (game's move t1,v0), r = tt - ...; h pinned $3 so branch 2's h->p.whole store matches; branch 1 uses s2 = sx >> 24 laundered. Left: sx built from t1 instead of v0 and scheduled after the f & 0x10 branch, branch 1 folds h - 1 - s into h - (s + 1) and does not cross-jump into branch 2's store, stray move a1,v1 in the fl & 0xff00 delay slot, loop-end n load (lhu t0 vs lh + move t0). Tried: tt/xm/sx orders, launders on xm/sx/w, w = h - 1 split (78-87), char/signed char xm (127-129).
// opusP: still 82. itbrute pass with priority bumps on bs/d/b/xm/sx/x/c/a/r/k/w, asm(""), asm::r(xm), asm::r(b) at every line: no gain.
// wip score 82 (opusO: itbrute priority bumps asm("" : "=r"(v) : "0"(v)) on a after c = *D++, r twice after its computation, x before the f & 0x10 test; was 122). was 155: ys and x and cnt unpinned (game's temps: sll v0 / sra s5, addiu v0,s4,1 + move), x = t - s via fresh int t (keeps (x-1)-sx order), second sx>>24 from an asm-laundered copy s2 (game does two sra), lh for loop-end n via *(short *)D; D++, asm("" :: "r"(f)) after the SPAD store (game computes (f&0xe00)>>9 in v0, not in place). left: bs/d regs swapped (a1/v1), b's (short) recomputed in v1 in the game (ours in place in t1), xm copied to t1 (move t1,v0) in the game, branch-1 reg/sched so the two h->p.whole stores cross-jump, game's stray move a1,v1 in the fl&0xff00 beq delay slot, n load move t0 at loop end.
// older: wip score 155 (was 201): s0..s7 pinned to the game's ret/x/o/f1/cnt/ys/fl/y and f/b/c to a3/t1/a0; fl branches swapped (getCollisionPlaneAt in the else), (short)r >= 0 -> goto hit. sx = xm << 24 computed once, `sx >> 24` per use (xm asm-laundered no longer needed). left: gcc folds x - 1 - sx into x - (sx + 1) and x + 8 - xm into x - (xm - 8) (game keeps (x-1)-sx, (x+8)-t1 with xm copied to t1); bs/d swapped (a1/v1); game computes c & 0xf before xm; loop-end n load is lh + move t0 in the game
#include "spad.h"
#include "TOBJ.H"

extern unsigned short *getCollisionPlaneAt(int, int);
extern unsigned short *D_1F800278;
extern unsigned short D_1F800282;
extern unsigned char *D_8007B5AC;
#define D D_1F800278

int func_800402EC(TObj *o, int x_, int y_, int fl_)
{
    int x = x_;
    register int y asm("$23") = y_;
    register int fl asm("$22") = fl_;
    register int ret asm("$16");
    int ys;
    register int f1 asm("$19");
    short cnt;
    short n;
    unsigned short a;
    register unsigned short f asm("$7");
    register unsigned short b asm("$9");
    register unsigned short c asm("$4");
    short bs, d;
    int r, k;
    register unsigned int xm asm("$9");
    unsigned char *t;
    register Fix16 *h asm("$3");
    unsigned short w;
    int sx;
    char pad[24];

    n = *D++;
    if (n == 0)
        return 0;
    if (0) {
    hit:
        (*(unsigned short*)&SPAD.s284) = (f & 0xe00) >> 9;
        D_1F800282 = f;
        o->h->p.whole -= r;
        return k;
    }
    cnt = 0;
    ret = 0;
    ys = (short)y_;
    f1 = fl_ & 1;
    do {
        f = *D++;
        n--;
        if ((f & 0xc) == 0) {
            D += 3;
            continue;
        }
        a = *D++;
        b = *D++;
        c = *D++;
        bs = b;
        if (bs == 0)
            continue;
asm("" : "=r"(a) : "0"(a));
        d = y - a;
        if (bs < 0) {
            if (d < bs)
                continue;
            if (d > 0)
                return ret;
        } else {
            if (bs < d)
                return ret;
            if (d < 0)
                continue;
        }
{ register int tt asm("$2") = x & 7; r = tt - (c & 0xf) - ((c >> 4) & 0xf) * (ys - (short)a) / (short)b; xm = tt; sx = tt << 24; }
        if (f & 0x10) {
asm("" : "=r"(sx) : "0"(sx));
            if (!f1 && (f & 4)) {
                { int t; t = x - 1; x = t - (sx >> 24); }
                ret = 1;
                h = o->h; { int s2 = sx >> 24; asm("" : "=r"(s2) : "0"(s2)); h->p.whole = h->p.whole - 1 - s2; }
            } else if (f & 8) {
                { int t; t = x + 8; x = t - xm; }
                ret = 2;
                h = o->h; h->p.whole = h->p.whole + 8 - xm;
            }
            (*(unsigned short*)&SPAD.s284) = (f & 0xe00) >> 9;
            asm("" :: "r"(f));
            cnt++;
            if (cnt >= 8)
                return ret;
            if ((fl & 0xff00) != 0xff00) {
asm("" : "=r"(x) : "0"(x));
                t = D_8007B5AC;
                t += *(unsigned short *)(t + 8);
                D = (unsigned short *)(t + *(unsigned short *)(t + ((short)x / 8) * 2));
            } else {
                D = getCollisionPlaneAt((short)x, o->d->p.whole);
            }
            n = *(short *)D; D++;
            continue;
        }
        if (!f1) {
            if (!(f & 4))
                continue;
asm("" : "=r"(r) : "0"(r));
            if ((short)r >= 0) {
                k = 1;
                goto hit;
            }
            return ret;
        } else {
            if (!(f & 8))
                continue;
asm("" : "=r"(r) : "0"(r));
            if ((short)r <= 0) {
                k = 2;
                goto hit;
            }
            return ret;
        }
    } while (n != 0);
    return ret;
}
