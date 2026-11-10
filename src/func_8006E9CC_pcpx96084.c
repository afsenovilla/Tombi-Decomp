// VERSION PCPX_96084 tomba 8006E9CC 96
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
typedef struct { unsigned short a, b, c, d; } Q;
extern short *D_800F4FFC;
extern Q D_800A7FF8[];
extern unsigned short D_800F5044, D_800F5046, D_800F5048, D_800F504A;

void func_8006E9CC_pcpx96084(void)
{
    unsigned short *p = &D_800A7FF8[D_800F4FFC[1]].a;
    D_800F5044 = *p++;
    D_800F5046 = *p++;
    D_800F5048 = *p;
    D_800F504A = *(volatile unsigned short *)(p + 1);
}
#endif
