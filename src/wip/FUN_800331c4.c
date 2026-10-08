// FUNC 800331c4 932 MAIN0
/* score 30 (b55, was 224): list[64] (frame 0x160), (unsigned short)k tests keep k (s1) set in the scan, final loop as
   for (i...) with do {} while (0) after k = 0x14, r int with u temp, branch order of h1/h2. Left: h2 in a0 instead of
   v1 inside the animFrame if; sort inner loop schedules sll before lh; k*2 folded to li 0x28 (game sll s6,s1,1:
   needs a CODE_LABEL between k = 0x14 and the loop for combine, plus k sign bits known; a label via &&lbl kept the
   extension); lw list[0] scheduled late. */
#include "TOBJ.H"
typedef struct P { short x, y; } P;
extern short DAT_1f80019e;
extern unsigned short DAT_1f80019eu;
extern unsigned short DAT_1f800250;
extern TObj **DAT_1f800260;
extern unsigned char DAT_8007a0a0[];
extern int FUN_8002078c(P, P);


int FUN_800331c4(TObj *o)
{
    P b, a;
    TObj *list[64];
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
                h2 = q->h;
                h1 = o->h;
            } else {
                h1 = q->h;
                h2 = o->h;
            }
            k = h1->p.whole - h2->p.whole;
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
        do {} while (0);
        a.x = o->h->p.whole;
        a.y = o->y.p.whole;
        for (i = 0; list[i] != 0; i++) {
            int r; int u;
            b.x = list[i]->h->p.whole;
            b.y = list[i]->y.p.whole;
            u = FUN_8002078c(a, b); r = u;
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
