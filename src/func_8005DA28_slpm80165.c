// VERSION SLPM_80165 slpm80165 8005DA28 104
#if defined(VERSION_SLPM_80165)
// Demo version: JP C of src/func_80064E1C_slps01144.c (func_80064E1C_slps01144) (1.000 masked) [-O2 -G0] (tools/demo_sweep.py)
extern unsigned char *D_8009BFE8_slpm80165;

int func_8005DA28_slpm80165(void)
{
    volatile int i, j, k; /* j, k unused: retail frame is 16 bytes */

    *(short *)(D_8009BFE8_slpm80165 + 10) = 0;
    i = 10;
    while (--i != -1) {
    }
    return 0;
}
#endif
