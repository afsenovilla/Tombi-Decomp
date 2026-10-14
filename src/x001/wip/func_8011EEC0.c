// FUNC 8011eec0 744 X001
/* score 2 (was 44): only loop 2's `addiu a1,o,165` sits 7 insns early (game: after `subu a2`).
   Fixes: final store as *((u8 *)o + n + 0xa4); loops 1/3 divisor via a loop-local `int d = k` (sra hoist/order);
   loop 3 stores through a block-local `Seg *m = s + d + i`; loop 4 `asm("")` between b = o + 0xa5 and b += k + i;
   loop 2 b = o + 0xa5 placed before e (with e first + asm("") the order is right but k/b regs swap and t6/t7 swap, 34).
   Tried for loop 2: asm/empty clobbers at every line, b/e/i/k asm reg-ties, $5..$25 pins for b (44+), int d = k,
   Seg *m local, &o->ba5, statement reorders (2..26), b += k; b -= i.
   Older: loop-4 bound n - (k + one) with `one = 1` assigned in the loop condition; last block e = s + (n - 1). */
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
        v = (e->a + e->a * D_8007A570[(i << 6) / d]) >> 12;
        (s + k - i)->a = v;
        (s + k - i)->c = v + 5;
    }
    for (i = 1; i <= k; i++) {
        unsigned char *b;
        b = (unsigned char *)o + 0xa5;
        e = s + k - i;
        b += k - i;
        e->b = e[1].a;
        e->d = e[1].c;
        *b = 1;
    }
    e = &s[k];
    for (i = 1; i < n - k; i++) {
        int d = k;
        v = (e->a + e->a * D_8007A570[(i << 6) / (n - d)]) >> 12;
        { Seg *m = s + d + i; m->a = v; m->c = v + 5; }
    }
    for (i = 1; one = 1, i < n - (k + one); i++) {
        unsigned char *b;
        e = s + k + i;
        b = (unsigned char *)o + 0xa5;
    asm("");
        b += k + i;
        e->b = e[1].a;
        e->d = e[1].c;
        *b = 2;
    }
    e = s + (n - 1);
    e->d = 5;
    e->b = 0;
    *((unsigned char *)o + n + 0xa4) = 2;
}
