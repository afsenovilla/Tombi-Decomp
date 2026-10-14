// FUNC 800261b0 140 MAIN0
// MATCHING 800261b0 140
/* The `addiu s0,sp,0x10` lands after the PadGetState call only when PadSetAct is prototyped as
   returning int (it does): the discarded result makes v0 set twice, so sched1 no longer boosts the
   value call ("birthing" insn) and the &align pseudo is scheduled below it (see notes). */
extern unsigned char D_8009D614;
extern unsigned char D_8009D615;
extern int PadSetAct(int, unsigned char *, int);
extern int PadGetState(int);
extern void PadSetActAlign(int, signed char *);

void stopPadVibration(void)
{
    signed char align[6] = {0, 1, 0xff, 0xff, 0xff, 0xff};
    D_8009D614 = 0;
    D_8009D615 = 0;
    PadSetAct(0, &D_8009D614, 2);
    if (PadGetState(0) == 6)
        PadSetActAlign(0, &align[0]);
}
