// VERSION PCPX_96084 tomba 80035130 88
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
extern short D_800DE31C;
extern unsigned char D_800DE420;

void func_80035130_pcpx96084(void)
{
    switch (D_800DE31C) {
    case 0: D_800DE420 = 1; break;
    case 1: case 2: case 3: D_800DE420 = 2; break;
    case 4: D_800DE420 = 3; break;
    }
}
#endif
