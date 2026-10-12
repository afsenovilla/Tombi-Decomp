// FUNC 80025d90 432 MAIN0
/* score 36 (was 100). n62: the block-0 ORDER of the game (andi a0 = c1&0xff before the pad lhu, `a-0x50` before
   `& 0xff0f`) is reproduced by: `pad0 = &DAT_8009d670; a1 = c1; d = a1 - 0x50; p = *pad0; pm = p & 0xff0f;
   m = pm | AXIS(...d...)` with ONE `d` variable reused by the second axis (multi-set => the addiu loses the
   birthing boost and is picked after the prio-3 andi; the andi whose consumer is in another block is never boosted
   because insns in the initial ready list skip adjust_priority). Left then (score 40) is only global-alloc order:
   game allocates m/pm (a2) before c2 (a1) and c1 (a3); ours gives c2 a2 (c2 3 refs/19 insns = 0.158 beats m 6/82
   = 0.146 and pm 2/40). Needs m born at the join (pm form) AND pm allocated before c1: no form found yet.
   Previous notes: logic, load/store order and the tail match; left: register choice in the first AXIS
   (game c1=a3, g=t0, a=a0, b=a1; ours c1=a1, g=a3, a=v1, b=a0). b17: extern volatile DAT_8009d674 (reloaded
   in the tail), unsigned short a = c with the final test on c (fresh andi at the label), first load + clear
   hoisted with a volatile second name for the clear (debt). Tried type brute force of c/a/m, a precomputed
   at several points, inline function instead of the macro.
   b46: -dl/-dg: a (75) gets v1 because the local temp a-0x50 (86) takes v0; in the game that temp is v1, so the raw
   pad lhu (v0) must still be live there, i.e. sched1 placed `& 0xff0f` after `a - 0x50`. Permutations of the 5 init
   statements, m = (pad & 0xff0f) | AXIS(..), block temps for the pad value / AXIS result: no gain (36-94). */
typedef struct { char p0[9]; unsigned char b9; } G;
extern unsigned char DAT_8009d618;
extern volatile unsigned char DAT_8009d618v;
extern G DAT_8009d610;
extern unsigned short DAT_8009d670;
extern volatile unsigned short DAT_8009d674;
extern unsigned short DAT_800a6114;
extern unsigned short DAT_1f8001fc;
extern unsigned short DAT_1f8001fe;

#define AXIS(v, c, a, hi, lo) ({                                  \
    unsigned short b;                                         \
    int r;                                                      \
    if ((unsigned)(a - 0x50) < 0x60) r = 0;                     \
    else {                                                      \
        b = a - 0x30;                                           \
        if (b < 0xa0) { (v) = 1; { int t = (b < 0x50) ? hi : lo; r = t; } }    \
        else {                                                  \
            b = a - 0x10;                                       \
            if (b < 0xe0) { (v) = 2; { int t = (b < 0x70) ? hi : lo; r = t; } } \
            else { (v) = 3; { int t = (c < 0x80) ? hi : lo; r = t; } }         \
        }                                                       \
    }                                                           \
    r; })

void FUN_80025d90(void)
{
    unsigned short m;
    unsigned char c1, c2;
    unsigned short a1, a2;
    volatile unsigned short *pad;
    G *g = &DAT_8009d610;
    c1 = DAT_8009d618;
    DAT_8009d618v = 0;
    DAT_8009d674 = DAT_800a6114;
    a1 = c1;
    m = *(volatile unsigned short *)&DAT_8009d670 & 0xff0f;
    m |= AXIS(DAT_8009d618, c1, a1, 0x80, 0x20);
    c2 = g->b9;
    g->b9 = 0;
    a2 = c2;
    m |= AXIS(g->b9, c2, a2, 0x10, 0x40);
    pad = &DAT_8009d670;
    *pad = m;
    DAT_1f8001fc = *pad & ~DAT_8009d674;
    DAT_1f8001fe = DAT_8009d674 & ~*pad;
    DAT_800a6114 = *pad;
}
