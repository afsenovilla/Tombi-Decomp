// VERSION PCPX_96084 tomba 80084868 204
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Event trigger test: the player stands at the trigger spot (y -739 and x >= 3065 once event bit 0x80 is set,
// else y >= -299 and x in 2586..2625); the first time also needs pad bit 0x80, then the latch +0x1C is set.
typedef struct { short frac, whole; } F16;
extern int D_800DE0FC;
extern short D_800F4FEE;
extern F16 *D_800F5018;
extern unsigned short D_1F8002B0;

int func_80084868_pcpx96084(char *o)
{
    int r = 0;
    if (D_800DE0FC & 0x80) {
        if (D_800F4FEE != -739) goto done;
        if (D_800F5018->whole < 3065) goto done;
    } else {
        if (D_800F4FEE < -299) goto done;
        if ((unsigned short)(D_800F5018->whole - 2586) >= 40) goto done;
    }
    if (*(unsigned short *)(o + 0x1c) == 0) {
        if (!(D_1F8002B0 & 0x80)) goto done;
        *(unsigned short *)(o + 0x1c) = 1;
    }
    r = 1;
done:
    return r;
}
#endif
