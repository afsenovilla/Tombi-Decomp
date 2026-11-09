// FUNC 8006bfa4 160 MAIN0
// MATCHING 8006bfa4 160
extern unsigned PAD_RC2_WAIT_START;
extern unsigned PAD_RC2_WAIT_LIMIT;
#define VAL (*(volatile unsigned short *)0x1f801120)
#define MODE (*(volatile unsigned short *)0x1f801124)
#define MAX (*(volatile unsigned short *)0x1f801128)

int chkRC2wait(void)
{
    unsigned u = VAL;
    if (u < PAD_RC2_WAIT_START) {
        if (MAX != 0)
            u += MAX;
        else
            u += 0x10000;
    }
    if (MODE & 0x200)
        return u - PAD_RC2_WAIT_START >= PAD_RC2_WAIT_LIMIT;
    else
        return (u - PAD_RC2_WAIT_START) >> 3 >= PAD_RC2_WAIT_LIMIT;
}
