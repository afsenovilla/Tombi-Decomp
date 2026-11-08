// VERSION SLPS_01144 dspsub 800F2FA0 16
#if defined(VERSION_SLPS_01144)
// JP copy of PAL d0spsub 800f18d0 (subScreenCondVar6102Set.c) with the retail variable name
extern unsigned char D_800A7C42;

int func_800F2FA0(void)
{
    return D_800A7C42 != 0;
}
#endif
