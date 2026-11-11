// VERSION BETA_980203 beta980203 80069C0C 152
#if defined(VERSION_BETA_980203)
// Beta version: PAL chkRC2wait.c with one non-volatile read of the max register (address held in v1), no cross-jump of the two adds (asm).
#define PAD_RC2_WAIT_START D_800A1E1C
#define PAD_RC2_WAIT_LIMIT D_800A64F8
extern unsigned PAD_RC2_WAIT_START;
extern unsigned PAD_RC2_WAIT_LIMIT;
#define VAL (*(unsigned short *)0x1f801120)
#define MODE (*(unsigned short *)0x1f801124)

int chkRC2wait(void)
{
    unsigned u = VAL;
    if (u < PAD_RC2_WAIT_START) {
        register unsigned short *p asm("$3") = (unsigned short *)0x1f801128;
        asm("" : "=r"(p) : "0"(p));
        if (*p != 0)
            u += *p;
        else
            { u += 0x10000; asm(""); }
    }
    if (MODE & 0x200)
        return u - PAD_RC2_WAIT_START >= PAD_RC2_WAIT_LIMIT;
    else
        return (u - PAD_RC2_WAIT_START) >> 3 >= PAD_RC2_WAIT_LIMIT;
}
#endif
