// VERSION BETA_980203 beta980203 80069BF0 28
// AS.BETA_980203 gnu
#if defined(VERSION_BETA_980203)
// Beta version: PAL setRC2wait.c with the root counter read as lui/lhu (D_1F801120), 28 bytes (PAL 32).
/* setRC2wait (libpad/waitrc2.c). Library code: GNU as fills the `lui at` / `sw lo(at)` delay slot. */
extern int PAD_RC2_WAIT_LIMIT;
extern int PAD_RC2_WAIT_START;
extern volatile unsigned short D_1F801120; /* root counter 2 count */
void setRC2wait(int a)
{
    int t = D_1F801120;
    PAD_RC2_WAIT_LIMIT = a;
    PAD_RC2_WAIT_START = t;
}
#endif
