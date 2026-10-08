// FUNC 8006bf84 32 MAIN0
/* score 5: game puts the 2nd global sw (lui at before jr, sw lo(at) in jr delay slot); gcc never fills a delay slot with a 2-insn sym store. Tried -fno-delayed-branch, -G4/-G8 (fail), -msplit-addresses (no such option); matchcheck same score. Library built with a newer compiler (gcc 2.8.1 also fails: score 11). */
extern int DAT_800a0a18;
extern int DAT_8009c33c;
void FUN_8006bf84(int a)
{
    int t = *(volatile unsigned short *)0x1f801120;
    DAT_800a0a18 = a;
    DAT_8009c33c = t;
}
