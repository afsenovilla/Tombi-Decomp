// FUNC 8006bfa4 160 MAIN0
// MATCHING 8006bfa4 160
extern unsigned DAT_8009c33c;
extern unsigned DAT_800a0a18;
#define VAL (*(volatile unsigned short *)0x1f801120)
#define MODE (*(volatile unsigned short *)0x1f801124)
#define MAX (*(volatile unsigned short *)0x1f801128)

int FUN_8006bfa4(void)
{
    unsigned u = VAL;
    if (u < DAT_8009c33c) {
        if (MAX != 0)
            u += MAX;
        else
            u += 0x10000;
    }
    if (MODE & 0x200)
        return u - DAT_8009c33c >= DAT_800a0a18;
    else
        return (u - DAT_8009c33c) >> 3 >= DAT_800a0a18;
}
