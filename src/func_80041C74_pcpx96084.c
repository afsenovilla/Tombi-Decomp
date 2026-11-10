// VERSION PCPX_96084 tomba 80041C74 112
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
extern int D_800DE0D8;
extern unsigned char D_800F4FD8[];
extern unsigned char D_800F4FDA;
extern void func_80055658(unsigned char *);
extern void updateObjectsLayer1(void);

void func_80041C74_pcpx96084(void)
{
    unsigned char *p = D_800F4FD8;
    if (D_800DE0D8 == 0x50001) D_800F4FDA = 1;
    else D_800F4FDA = 0;
    if (p[0] != 0) func_80055658(p);
    updateObjectsLayer1();
}
#endif
