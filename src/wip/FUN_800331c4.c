// FUNC 800331c4 932 MAIN0
/* score 224 (b48, was 271): scan loop as if ((x = D) != 0) { wx = 0x80; wy = 0x90; do {...} while ((x = D) != 0); }
   gives the game's duplicated lh+lhu test and hoisted t1/t0 constants. Game shares registers by variable reuse: s0 = i
   (init loop, sort inner loop, final loop), s1 = k (scan diff temp `k = a - b + 0x2d`, sort outer counter, final 0x14
   with `sll s6,s1,1` = k*2 not constant-folded). Open: ours folds k = 0x14 into li 0x14/li 0x28 (k never crosses the
   call, so k ends in t0 and the scan temps in v0); frame 0x180 vs 0x160 (32 B of extra vars: short temps?); s2/s1 swap
   for o. Tried k = 0x14 inside the loop body, chk8 as inline or open-coded. */
#include "TOBJ.H"
typedef struct P { short x, y; } P;
extern short DAT_1f80019e;
extern unsigned short DAT_1f80019eu;
extern unsigned short DAT_1f800250;
extern TObj **DAT_1f800260;
extern unsigned char DAT_8007a0a0[];
extern short FUN_8002078c(P, P);

static __inline__ int chk(int d, int w)
{
    return (d & 0xffff) < w;
}


int FUN_800331c4(TObj *o)
{
    P b, a;
    TObj *list[72];
    TObj **p;
    TObj *q, *t;
    Fix16 *h1, *h2;
    short i, m, n, c, k;
    short best;
    unsigned short u;
    int wx, wy;
    short x;

    p = DAT_1f800260;
    DAT_1f80019e = DAT_1f800250;
    for (i = 0; i < 0x40; i++)
        list[i] = 0;
    n = 0;
    if ((x = DAT_1f80019e) != 0) {
        wx = 0x80;
        wy = 0x90;
        do {
        q = *p++;
        DAT_1f80019e = x - 1;
        if ((q->active & 1) && DAT_8007a0a0[q->type]) {
            k = o->d->p.whole - q->d->p.whole + 0x2d;
            c = (unsigned short)k < 0x5a;
            if (o->animFrame & 1) {
                h1 = o->h;
                h2 = q->h;
            } else {
                h1 = q->h;
                h2 = o->h;
            }
            k = h1->p.whole - h2->p.whole;
            if (chk(k, wx))
                c++;
            k = o->y.p.whole - q->y.p.whole + 0x20;
            if (chk(k, wy))
                c++;
            if (c == 3) {
                list[n] = q;
                n++;
            }
        }
        } while ((x = DAT_1f80019e) != 0);
    }
    for (k = 0; k < n; k++) {
        m = k;
        best = list[k]->y.p.whole;
        for (i = k; i < n; i++) {
            if (best < list[i]->y.p.whole) {
                m = i;
                best = list[i]->y.p.whole;
            }
        }
        t = list[k];
        list[k] = list[m];
        list[m] = t;
    }
    if (n != 0) {
        k = 0x14;
        a.x = o->h->p.whole;
        a.y = o->y.p.whole;
        i = 0;
        if (list[0] != 0) {
            do {
                short r;
                b.x = list[i]->h->p.whole;
                b.y = list[i]->y.p.whole;
                r = FUN_8002078c(a, b);
                if ((((r - (unsigned short)o->waa) + k) & 0xff) < k * 2) {
                    o->wac = r;
                    if (o->y.p.whole < b.y)
                        return 2;
                    return 1;
                }
                i++;
            } while (list[i] != 0);
        }
    }
    return 0;
}
