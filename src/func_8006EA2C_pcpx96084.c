// VERSION PCPX_96084 tomba 8006EA2C 160
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// State 0: func_8006E768; state 1: the body of func_8006E9CC_pcpx96084 (inlined).
typedef struct { unsigned short a, b, c, d; } Q8006EA2C;
extern unsigned char D_800F4FDA;
extern short *D_800F4FFC;
extern Q8006EA2C D_800A7FF8[];
extern unsigned short D_800F5044, D_800F5046, D_800F5048, D_800F504A;
extern void func_8006E768(void);

static __inline__ void loadQuad(void)
{
    unsigned short *p = &D_800A7FF8[D_800F4FFC[1]].a;
    D_800F5044 = *p++;
    D_800F5046 = *p++;
    D_800F5048 = *p;
    D_800F504A = *(volatile unsigned short *)(p + 1);
}

void func_8006EA2C_pcpx96084(void)
{
    switch (D_800F4FDA) {
    case 0:
        func_8006E768();
        break;
    case 1:
        loadQuad();
        break;
    }
}
#endif
