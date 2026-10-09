// FUNC 8011eec0 744 X001
/* score 121 (o32): prologue fixed (s itself advanced in place), loops 2/4 via e = s + k -/+ i and a byte pointer
   b = (u8 *)o + 0xa5; b += k -/+ i (game form). Left: loop-4 bound: game computes k + 1 then n - (k + 1) (pre-test
   from the raw b6b byte t5 + 1), but fold turns n - (k + 1) into (n - 1) - k; an `int one = 1` variable gives the
   game shape (83) but takes t8 and i = 1 reuses it; ({k + 1;}) / inline inc(k) give it too (113) but lose the 8-byte
   frame. Also e/i registers swapped (a3/a2) and last block s + n - 1 folded into offsets. */
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
        v = (e->a + e->a * D_8007A570[(i << 6) / k]) >> 12;
        (s + k - i)->a = v;
        (s + k - i)->c = v + 5;
    }
    for (i = 1; i <= k; i++) {
        unsigned char *b;
        e = s + k - i;
        b = (unsigned char *)o + 0xa5;
        b += k - i;
        e->b = e[1].a;
        e->d = e[1].c;
        *b = 1;
    }
    e = &s[k];
    for (i = 1; i < n - k; i++) {
        v = (e->a + e->a * D_8007A570[(i << 6) / (n - k)]) >> 12;
        (s + k + i)->a = v;
        (s + k + i)->c = v + 5;
    }
    for (i = 1; i < n - k - 1; i++) {
        unsigned char *b;
        e = s + k + i;
        b = (unsigned char *)o + 0xa5;
        b += k + i;
        e->b = e[1].a;
        e->d = e[1].c;
        *b = 2;
    }
    e = s + n - 1;
    e->d = 5;
    e->b = 0;
    U8(o, 0xa4 + n) = 2;
}
