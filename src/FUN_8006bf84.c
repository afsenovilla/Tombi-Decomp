// FUNC 8006bf84 32 MAIN0
// MATCHING 8006bf84 32
// FLAGS -O2 -G0
// AS gnu
/* setRC2wait (libpad/waitrc2.c). Library code: GNU as fills the `lui at` / `sw lo(at)` delay slot. */
extern int DAT_800a0a18;
extern int DAT_8009c33c;
void FUN_8006bf84(int a)
{
    int t = *(volatile unsigned short *)0x1f801120;
    DAT_800a0a18 = a;
    DAT_8009c33c = t;
}
