// VERSION SLPS_01144 dspsub3 800EFE08 16
#if defined(VERSION_SLPS_01144)
// JP copy of PAL d0spsub 800f18c0 (subScreenCondVarD0E6Under5.c) with the retail variable name
extern unsigned char D_8009EC8E;

int func_800EFE08(void)
{
    return D_8009EC8E < 5;
}
#endif
