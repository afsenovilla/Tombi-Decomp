// VERSION SLPM_80165 slpm80165 8005D7D8 16
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo-only setter, like the JP CD_set_test_parmnum (src/CD_set_test_parmnum_slps01144.c).
extern int D_8008742C;
void func_8005D7D8_slpm80165(int a)
{
    D_8008742C = a;
}
#endif
