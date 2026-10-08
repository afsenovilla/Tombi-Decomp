// FUNC 800331c4 932 MAIN0
/* score 271: logic complete. Open: (1) game duplicates the while(DAT_1f80019e) test at the top (lh+lhu) and hoists
   li 0x80/0x90 (scan loop) and li 0x14 / sll 0x28 (final loop) into s-regs: constants live in registers, maybe
   locals assigned before the loops or inline params; ours cross-jumps the top test into a `j`. (2) frame 0x160 vs ours. */
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

static __inline__ int chk8(int d, int w)
{
    return ((d + w) & 0xff) < w * 2;
}

int FUN_800331c4(TObj *o)
{
    P b, a;
    TObj *list[72];
    TObj **p;
    TObj *q, *t;
    Fix16 *h1, *h2;
    short i, j, m, n, c, k;
    short best;
    unsigned short u;
    int wx, wy, wr;

    p = DAT_1f800260;
    DAT_1f80019e = DAT_1f800250;
    for (i = 0; i < 0x40; i++)
        list[i] = 0;
    n = 0;
    wx = 0x80;
    wy = 0x90;
    while (DAT_1f80019e != 0) {
        q = *p++;
        DAT_1f80019e--;
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
            if (chk(h1->p.whole - h2->p.whole, wx))
                c++;
            if (chk(o->y.p.whole - q->y.p.whole + 0x20, wy))
                c++;
            if (c == 3)
                list[n++] = q;
        }
    }
    for (k = 0; k < n; k++) {
        m = k;
        best = list[k]->y.p.whole;
        for (j = k; j < n; j++) {
            if (best < list[j]->y.p.whole) {
                m = j;
                best = list[j]->y.p.whole;
            }
        }
        t = list[k];
        list[k] = list[m];
        list[m] = t;
    }
    if (n != 0) {
        wr = 0x14;
        a.x = o->h->p.whole;
        a.y = o->y.p.whole;
        i = 0;
        if (list[0] != 0) {
            do {
                short r;
                b.x = list[i]->h->p.whole;
                b.y = list[i]->y.p.whole;
                r = FUN_8002078c(a, b);
                if (chk8(r - (unsigned short)o->waa, wr)) {
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
