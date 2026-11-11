// VERSION PCPX_96084 tomba 80025944 72
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
typedef struct { int a, b; } Pair;
extern int D_800DE0E0;
extern Pair D_800E24D0[];
extern Pair D_800A24E8[];

void func_80025944_pcpx96084(void)
{
    int i;
    D_800DE0E0 = 0;
    for (i = 0; i < 10; i++) {
        D_800E24D0[i] = D_800A24E8[i];
    }
}
#endif
