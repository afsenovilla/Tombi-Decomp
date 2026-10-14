// FUNC 8011eec0 744 X001
/* score 44 (o32): loop-4 bound n - (k + one) with `one = 1` assigned in the loop condition (cse turns it into
   addiu t5,1 / addiu v1,1 without fold); last block e = s + (n - 1) (game n*40 - 40 + s). Earlier: prologue (s
   advanced in place), loops 2/4 via e = s + k -/+ i and byte pointer b = (u8 *)o + 0xa5; b += k -/+ i.
   Left: loops 2/4 regs (game k=a0, b=a1, 2nd lhu v1/a0; ours rotated), loop 1 sra k scheduled after sll i,
   loop 3 k (sra a1,t3) hoisted out of loop in game, loop 3 s+k*40 operand order, o+n order at the end.
   Pinning b to $5 worsens (66+). */
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
    for (i = 1; one = 1, i < n - (k + one); i++) {
        unsigned char *b;
        e = s + k + i;
        b = (unsigned char *)o + 0xa5;
        b += k + i;
        e->b = e[1].a;
        e->d = e[1].c;
        *b = 2;
    }
    e = s + (n - 1);
    e->d = 5;
    e->b = 0;
    U8(o, 0xa4 + n) = 2;
}
