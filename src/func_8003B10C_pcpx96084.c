// VERSION PCPX_96084 tomba 8003B10C 44
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 version: SLPM_80165 C of src/func_801163B8_slpm80165.c (func_801163B8_slpm80165, twin score 0.909) [-O2 -G0] (tools/demo_sweep.py)
// Demo-only helper: advances the frame (+0x2e) by 2 up to 143.
void func_801163B8_slpm80165(char *o)
{
    *(unsigned short *)(o + 0x2e) += 2;
    if (*(unsigned short *)(o + 0x2e) >= 144)
        *(unsigned short *)(o + 0x2e) = 143;
}
#endif
