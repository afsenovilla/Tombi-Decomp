// FUNC 800331c4 932 MAIN0
// MATCHING 800331c4 932
/* The dead code after `k = 0x14` (two branches on n, LX, T) is a cse2/combine construct (debt): the game hoists
   `sll s6,s1,1` (k*2) unfolded; plain C folds it because cse2 sees k = 0x14 and the loop pre-header in one extended
   basic block. The `do {} while (0)` hides the outer n-test from cse1 only (cse1 stops at NOTE_INSN_LOOP_END, cse2
   does not); cse2 then folds `n == 0` (never taken: LX loses its use) and `n != 0` (always taken: becomes `j T`
   and the `return 0` tail is deleted), and the block after the barrier (LX) starts a fresh cse table, so k*2 stays
   a shift; jump2 deletes the LX dead code and `j T` (jump to next), so T is gone before combine and k's
   sign extension can be dropped in the same label region. See notes/matching_guide.md (ninth batch). */
#include "TOBJ.H"
typedef struct P { short x, y; } P;
extern short DAT_1f80019e;
extern unsigned short DAT_1f80019eu;
extern unsigned short DAT_1f800250;
extern TObj **DAT_1f800260;
extern unsigned char DAT_8007a0a0[];
extern int angleBetweenPoints(P, P);


int FUN_800331c4(TObj *o)
{
    P b, a;
    TObj *list[64];
    TObj **p;
    TObj *q, *t;
    Fix16 *h1, *h2;
    short i, m, n, c, k;
    short best;
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
            if (o->animFrame & 1)
                k = o->h->p.whole - q->h->p.whole;
            else
                k = q->h->p.whole - o->h->p.whole;
            if ((unsigned short)k < wx)
                c++;
            k = o->y.p.whole - q->y.p.whole + 0x20;
            if ((unsigned short)k < wy)
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
            if (list[i]->y.p.whole > best) {
                m = i;
                best = list[i]->y.p.whole;
            }
        }
        t = list[k];
        list[k] = list[m];
        list[m] = t;
    }
    if (n != 0) {
        do {} while (0);
        k = 0x14;
        if (n == 0) goto LX;
        if (n != 0) goto T;
        return 0;
    LX:
        if (i != 0) goto T;
        n--;
    T:
        a.x = o->h->p.whole;
        a.y = o->y.p.whole;
        for (i = 0; list[i] != 0; i++) {
            int r; int u;
            b.x = list[i]->h->p.whole;
            b.y = list[i]->y.p.whole;
            u = angleBetweenPoints(a, b); r = u;
            u = u - (unsigned short)o->waa; if ((unsigned char)(u + k) < k * 2) {
                o->wac = r;
                if (o->y.p.whole < b.y)
                    return 2;
                return 1;
            }
        }
    }
    return 0;
}
