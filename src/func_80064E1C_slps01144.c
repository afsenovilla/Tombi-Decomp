// VERSION SLPS_01144 slps01144 80064E1C 104
// FLAGS -O2 -G0
#if defined(VERSION_SLPS_01144)
extern unsigned char *D_8009BFE8;

int func_80064E1C_slps01144(void)
{
    volatile int i, j, k; /* j, k unused: retail frame is 16 bytes */

    *(short *)(D_8009BFE8 + 10) = 0;
    i = 10;
    while (--i != -1) {
    }
    return 0;
}
#endif
