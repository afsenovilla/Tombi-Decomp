// FUNC 800331c4 932 MAIN0
/* score 2 (b55, was 224): list[64] (frame 0x160), (unsigned short)k tests keep k (s1) set in the scan, the h test as
   one full expression per animFrame branch, final loop as for (i...), r int with u temp. Left: game hoists
   `sll s6,s1,1` (k*2 not folded, no sign extension); ours folds k = 0x14 into li 0x28 (combine knows k's last value:
   no CODE_LABEL between k = 0x14 and the loop). A label (static &&lbl) keeps k but leaves sll 16 / sra 15 because the
   scan and sort sets of k kill its sign-bit info; a separate single-set var for 0x14 + label gives the right
   `sll x,y,1` but then k (scan/sort) leaves s1. Tried RHS forms k<<1, k+k, casts; register asm (worse). */#include "TOBJ.H"
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
        k = 0x14;
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
