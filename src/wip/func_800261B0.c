// FUNC 800261b0 140 MAIN0
/* score 8: only the `addiu s0,sp,0x10` (&align pseudo, created by instantiation and reused by CSE as the call arg) lands in the 2nd jal delay slot; game has it after that call (bne delay slot). b31 tried: explicit p at every statement position, inner-block p, early return, arg/array types, callee protos, D_8009D614 as array (17), store order, FLAGS variants (-fno-schedule-insns2 etc.): all 8.
   b45: sched2 trace: at T-4 {8 (s0=sp+16), 31 (a0=0)} the class rule picks 8 (anti-dep of the call) over 31 (data dep),
   so 8 lands next to the jal; insns cannot cross calls and reorg stops at a filled call, so 8 must precede 31 in the
   RTL order. Also tried: struct copy B buf = DAT_80010238 like sibling FUN_80026000, r = call; if (r == 6). */
/* b52: sched1 (-dS) is where it goes wrong: insn 8 (s0=sp+16, emitted at the top by the aggregate init) and 37 (li v1,6) / 33 (call) are all boosted ready at T-2/T-3 and the tie goes to the higher luid, so 8 stays before the call. reg_n_calls_crossed comes from flow before sched1, so the game's s0 is consistent with sched1 having moved insn 8 below the call. p=align at every position, B struct copy via pointer, r temp, early return, switch: all 9.
   o39: sched1 backward list: at T-2 ready {8, 37 (li v1,6)} tie goes to the higher luid (37, emitted by the branch's
   force_reg), so insn 8 must sit AFTER the li in RTL at sched1 time. A variable `int six = 6` declared before the init
   gives the game's order (li; addiu s0; bne) but then six lives in an s-reg across the calls (32). Also tried copy after
   the stores, p in branch, struct-in-struct, char[6] cast copy, -fno-schedule-insns, -O1: 9 or worse. */
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
