// VERSION SLPM_80165 slpm80165 80022008 32
#if defined(VERSION_SLPM_80165)
// Demo version: JP C of src/func_80022B00_slps01144.c (func_80022B00_slps01144) (1.000 masked) [-O2 -G0] (tools/demo_sweep.py)
unsigned char func_80022008_slpm80165(unsigned char a, unsigned char b)
{
    unsigned char r;
    int d = a - b;

    r = d;
    if (d < 0) {
        r = 0;
    }
    return r;
}
#endif
