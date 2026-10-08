/* score 42 (b30, was 68): r = 0 moved from before `if (w & 0x10)` to just before the switch (game re-sets r=0 in the
   failure block, so the initial r=0 must not reach it or CSE deletes it); now r=a0, m=a0, lo copy v1, hi a2 like the game.
   Left: (1) short inr result in v0 copied to r (`move a0,v0` cross-jumped with the call-result copy); an int inline puts
   it straight in r but becomes slt/xori; (2) game has `move a0,zero` in the beqz (w&0x10) delay slot and slti in the
   switch beq slot, ours steals andi / puts r=0 in the beq slot. Tried (b30): int/long/char inline returns x 6 bodies
   (goto no, &&, nested), r short, r=0 at the else start / default case / before the if, open-coded tests (90).
   Earlier (b16): type brute force, chk() whole-test inline (90). */
// FUNC 80040f78 596 MAIN0
extern unsigned short *DAT_1f800278;
extern unsigned short DAT_1f800282, DAT_1f800284;
extern int FUN_8004094c(int, int);
extern int FUN_80040d30(int, int);
extern int func_80040AC0(int, int);
extern int func_80040BFC(int, int);

static __inline__ short inr(short m, short lo, short hi)
{
    if (m < lo) return 0;
    if (lo + hi < m) return 0;
    return 1;
}
int func_80040F78(void *o, int x, int y)
{
    short n, m;
    int r;
    unsigned short w, a, b, c;
    int lo, hi;
    unsigned short s;
    n = *(short *)DAT_1f800278++;
    if (n == 0)
        return 0;
    do {
        w = *DAT_1f800278++;
        n--;
        if ((w & 0x1f) == 0) {
            DAT_1f800278 += 3;
            continue;
        }
        if (w & 0x10) {
            DAT_1f800284 = (w & 0xe00) >> 9;
            a = *DAT_1f800278++;
            b = *DAT_1f800278++;
            c = *DAT_1f800278++;
            lo = c & 0xf;
            hi = (c >> 4) & 0xf;
            s = a + b;
            if ((short)a < (short)y || (short)y < (short)s)
                r = 0;
            else
                r = inr((short)x % 8, lo, hi);
        } else {
            DAT_1f800284 = (w & 0xe00) >> 9;
            r = 0;
            switch (w & 0xf) {
            case 1: r = FUN_8004094c((short)x, (short)y); break;
            case 2: r = FUN_80040d30((short)x, (short)y); break;
            case 4: r = func_80040AC0((short)x, (short)y); break;
            case 8: r = func_80040BFC((short)x, (short)y); break;
            }
        }
        if (r) {
            DAT_1f800282 = w;
            return 1;
        }
    } while (n);
    return 0;
}
