// JP-only (x01), written from the disassembly
// VERSION SLPS_01144 x01 8011DDE4 768
// FLAGS -O2 -G0
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"
#include "raw7.h"

typedef struct {
    short w0[11];
    short a;    /* 0x16 */
    short w18[3];
    short b;    /* 0x1e */
    short w20;
    short f;    /* 0x22 */
    short w24;
    short c;    /* 0x26 */
    short w28[3];
    short d;    /* 0x2e */
    short w30[2];
} Seg;

extern short D_800770D0[];

void func_8011DDE4_slps01144(TObj *o)
{
    Seg *s;
    Seg *e;
    short n, k;
    int i, v;
    int one;

    s = (Seg *)o->da0;
    k = o->b6b;
    s = (Seg *)((int *)s + 1);
    n = *(int *)s;
    s = (Seg *)((int *)s + 1);
    if (k == 0) return;
    if (k == n - 1) return;
    e = s;
    e->c = 5;
    e->a = 0;
    e += k;
    for (i = 1; i < k; i++) {
        int d = k;
        v = (e->a + e->a * D_800770D0[(i << 6) / d]) >> 12;
        (s + k - i)->a = v;
        (s + k - i)->c = v + 5;
    }
    for (i = 1; i <= k; i++) {
        e = s + k - i;
        e->b = e[1].a;
        e->d = e[1].c;
        e->f = 1;
    }
    e = &s[k];
    for (i = 1; i < n - k; i++) {
        int d = k;
        Seg *m = s + k + i;
        v = (e->a + e->a * D_800770D0[(i << 6) / (n - d)]) >> 12;
        m->a = v;
        m->c = v + 5;
    }
    for (i = 1; one = 1, i < n - (k + one); i++) {
        e = s + k + i;
        e->b = e[1].a;
        e->d = e[1].c;
        e->f = 2;
    }
    e = s + (n - 1);
    e->d = 5;
    e->b = 0;
    e->f = 2;
}
#endif
