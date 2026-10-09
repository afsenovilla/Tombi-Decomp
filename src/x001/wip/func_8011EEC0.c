/* score 181: logic believed right (4 loops over 40-byte rope segments); loop strength reduction differs: game keeps k<<16 and recomputes s + k*40 each iteration minus a 40*i giv, uses t-regs for the table base/giv; register naming shifted (t3/t4/t5). Not tuned further. */
// FUNC 8011eec0 744 X001
#include "TOBJ.H"
#include "raw7.h"

typedef struct {
    short w0[5];
    short a;    /* 0x0a */
    short w0c[3];
    short b;    /* 0x12 */
    short w14[3];
    short c;    /* 0x1a */
    short w1c[3];
    short d;    /* 0x22 */
    short w24[2];
} Seg;

extern short D_8007A570[];

void func_8011EEC0(TObj *o)
{
    int *q;
    Seg *s;
    Seg *e;
    short n, k;
    int i, v;

    q = (int *)o->da0 + 1;
    k = o->b6b;
    n = *q++;
    s = (Seg *)q;
    if (k == 0) return;
    if (k == n - 1) return;
    e = s;
    e->c = 5;
    e->a = 0;
    e += k;
    for (i = 1; i < k; i++) {
        v = (e->a + e->a * D_8007A570[(i << 6) / k]) >> 12;
        (s + k - i)->a = v;
        (s + k - i)->c = v + 5;
    }
    for (i = 1; i <= k; i++) {
        s[k - i].b = s[k - i + 1].a;
        s[k - i].d = s[k - i + 1].c;
        U8(o, 0xa5 + (k - i)) = 1;
    }
    e = &s[k];
    for (i = 1; i < n - k; i++) {
        v = (e->a + e->a * D_8007A570[(i << 6) / (n - k)]) >> 12;
        s[k + i].a = v;
        s[k + i].c = v + 5;
    }
    for (i = 1; i < n - (k + 1); i++) {
        s[k + i].b = s[k + i + 1].a;
        s[k + i].d = s[k + i + 1].c;
        U8(o, 0xa5 + (k + i)) = 2;
    }
    s[n - 1].d = 5;
    s[n - 1].b = 0;
    U8(o, 0xa4 + n) = 2;
}
