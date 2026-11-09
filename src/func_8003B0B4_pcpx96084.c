// VERSION PCPX_96084 tomba 8003B0B4 44
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 version: SLPM_80165 C of src/func_80116360_slpm80165.c (func_80116360_slpm80165, twin score 0.909) [-O2 -G0] (tools/demo_sweep.py)
// Demo-only helper: steps the frame (+0x2e) towards 135.
void func_80116360_slpm80165(char *o)
{
    unsigned short x = *(unsigned short *)(o + 0x2e);
    if (x != 135) {
        if (x < 135) *(unsigned short *)(o + 0x2e) = x + 1;
        else *(unsigned short *)(o + 0x2e) = x - 1;
    }
}
#endif
