// VERSION PCPX_96084 tomba 8003B094 32
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 version: SLPM_80165 C of src/func_80116340_slpm80165.c (func_80116340_slpm80165, twin score 0.875) [-O2 -G0] (tools/demo_sweep.py)
// Demo-only helper: counts +0x2e up to 143.
void func_80116340_slpm80165(char *o)
{
    if (*(unsigned short *)(o + 0x2e) != 143)
        (*(unsigned short *)(o + 0x2e))++;
}
#endif
