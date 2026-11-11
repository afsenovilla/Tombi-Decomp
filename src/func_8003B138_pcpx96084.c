// VERSION PCPX_96084 tomba 8003B138 44
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 version: SLPM_80165 C of src/func_801163E4_slpm80165.c (func_801163E4_slpm80165, twin score 0.909) [-O2 -G0] (tools/demo_sweep.py)
// Demo-only helper: advances the frame (+0x2e) by 2 up to 135.
void func_801163E4_slpm80165(char *o)
{
    *(unsigned short *)(o + 0x2e) += 2;
    if (*(unsigned short *)(o + 0x2e) >= 136)
        *(unsigned short *)(o + 0x2e) = 135;
}
#endif
