// VERSION PCPX_96084 tomba 80043478 164
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Brings D_800F5082 back towards 0: steps of 6, 16 or 17 by distance, 0 once within 17.
extern short D_800F5082;

void func_80043478_pcpx96084(void)
{
    short *p = &D_800F5082;
    short v = *p;
    if (v > -18 && v < 18) *p = 0;
    else if (v > 324) *p = v - 6;
    else if (v > 132) *p = v - 16;
    else if (v > 0) *p = v - 17;
    else if (v < -324) *p = v + 6;
    else if (v < -132) *p = v + 16;
    else if (v < 0) *p = v + 17;
}
#endif
