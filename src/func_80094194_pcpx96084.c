// VERSION PCPX_96084 tomba 80094194 64
#if defined(VERSION_PCPX_96084)
// PrePre 8 version: JP C of src/func_80064E84_slps01144.c (func_80064E84_slps01144) (1.000 masked) [-O2 -G0] (tools/demo_sweep.py)
extern int *D_8009BFEC_pcpx96084;
int func_80094194_pcpx96084(void)
{
    int *p = D_8009BFEC_pcpx96084;

    if (p[1] & 1) {
        if (p[0] & 1) goto one;
    }
    return 0;
one:
    return 1;
}
#endif
