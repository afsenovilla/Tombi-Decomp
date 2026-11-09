// VERSION SLPS_01144 slps01144 80064E84 64
// FLAGS -O2 -G0
#if defined(VERSION_SLPS_01144)
extern int *D_8009BFEC;
int func_80064E84_slps01144(void)
{
    int *p = D_8009BFEC;

    if (p[1] & 1) {
        if (p[0] & 1) goto one;
    }
    return 0;
one:
    return 1;
}
#endif
