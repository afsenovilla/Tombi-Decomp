// FUNC 8006bf84 32 MAIN0
// MATCHING 8006bf84 32
// FLAGS -O2 -G0
// AS gnu
/* setRC2wait (libpad/waitrc2.c). Library code: GNU as fills the `lui at` / `sw lo(at)` delay slot. */
extern int PAD_RC2_WAIT_LIMIT;
extern int PAD_RC2_WAIT_START;
void setRC2wait(int a)
{
    int t = *(volatile unsigned short *)0x1f801120;
    PAD_RC2_WAIT_LIMIT = a;
    PAD_RC2_WAIT_START = t;
}
