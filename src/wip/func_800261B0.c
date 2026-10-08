// FUNC 800261b0 140 MAIN0
/* score 8: only the `addiu s0,sp,0x10` (&align pseudo, created by instantiation and reused by CSE as the call arg) lands in the 2nd jal delay slot; game has it after that call (bne delay slot). b31 tried: explicit p at every statement position, inner-block p, early return, arg/array types, callee protos, D_8009D614 as array (17), store order, FLAGS variants (-fno-schedule-insns2 etc.): all 8.
   b45: sched2 trace: at T-4 {8 (s0=sp+16), 31 (a0=0)} the class rule picks 8 (anti-dep of the call) over 31 (data dep),
   so 8 lands next to the jal; insns cannot cross calls and reorg stops at a filled call, so 8 must precede 31 in the
   RTL order. Also tried: struct copy B buf = DAT_80010238 like sibling FUN_80026000, r = call; if (r == 6). */
extern unsigned char D_8009D614;
extern unsigned char D_8009D615;
extern void func_80069410(int, unsigned char *, int);
extern int FUN_80069050(int);
extern void func_80069390(int, signed char *);

void func_800261B0(void)
{
    signed char align[6] = {0, 1, 0xff, 0xff, 0xff, 0xff};
    D_8009D614 = 0;
    D_8009D615 = 0;
    func_80069410(0, &D_8009D614, 2);
    if (FUN_80069050(0) == 6)
        func_80069390(0, &align[0]);
}
