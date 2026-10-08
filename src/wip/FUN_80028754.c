// FUNC 80028754 368 MAIN0
/* b28: score 72 with short *py = o + 0x32 (puts addiu sp first and an entry insn in the bne slot, like the game). Root cause of the rest: cc1 -dg shows o's pseudo has a hard-reg preference for a0, so global-alloc makes n avoid a0; in the game n takes a0 and o (no a0 preference) ends in t0, all of n/s/E/const shift down one reg. Whole body as inline(o) still coalesces o into a0.
   w5: score 103, rewritten from the mirror function func_800285EC (matched). Body matches except the game moves o to t0 (move t0,a0 in the first bne slot, addiu sp first), so n gets a0; inline wrappers/local copies of o did not help. b25: in the game n is allocated before o (global-alloc priority), so n takes a0 and o ends up last in t0; mirror body as static __inline__ inl(o) gives 111, type greedy on n/s/r 108. */
typedef struct { char p[0x32]; short y; } TObj;
extern unsigned char DAT_800a60d6;
extern int DAT_800a606c;
extern unsigned short F2u;
extern short F2;
extern unsigned short DAT_1f80016e;
extern short E[];
extern short E2;

static __inline__ int calc(void)
{
    int t;
    if (DAT_800a60d6 == 3) {
        t = F2u - 0x14;
        return t - DAT_800a606c;
    }
    return F2u - DAT_1f80016e;
}

void FUN_80028754(TObj *o)
{
    char pad;
    short n;
    int s;
    int r;
    short *py = (short *)((char *)o + 0x32);
    r = calc();
    s = r;
    r = 0;
    n = s;
    n += E[0];
    if (n != 0x3a) {
        if (n < 0x3a) {
            if (n < -6) {
                F2 += 2;
                if (*py < F2) F2 = *py;
                else s += 2;
            } else {
                E[0] += 2;
            }
            if ((short)s + E[0] > 0x3a)
                E[0] = 0x3a - s;
        } else {
            E[0] -= 2;
            if ((short)s + E[0] < 0x3a)
                E[0] = 0x3a - s;
        }
    }
    if (*py < F2 + E2)
        E2 = *py - F2;
}
