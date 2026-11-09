// VERSION SLPM_80165 slpm80165 8005DA90 64
#if defined(VERSION_SLPM_80165)
// Demo version: JP C of src/func_80064E84_slps01144.c (func_80064E84_slps01144) (1.000 masked) [-O2 -G0] (tools/demo_sweep.py)
extern int *D_8009BFEC_slpm80165;
int func_8005DA90_slpm80165(void)
{
    int *p = D_8009BFEC_slpm80165;

    if (p[1] & 1) {
        if (p[0] & 1) goto one;
    }
    return 0;
one:
    return 1;
}
#endif
