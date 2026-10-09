// FUNC 800261b0 140 MAIN0
// MATCHING 800261b0 140
/* The `addiu s0,sp,0x10` lands after the FUN_80069050 call only when func_80069410 is prototyped as
   returning int (it does): the discarded result makes v0 set twice, so sched1 no longer boosts the
   value call ("birthing" insn) and the &align pseudo is scheduled below it (see notes). */
extern unsigned char D_8009D614;
extern unsigned char D_8009D615;
extern int func_80069410(int, unsigned char *, int);
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
