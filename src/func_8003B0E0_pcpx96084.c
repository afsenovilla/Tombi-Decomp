// VERSION PCPX_96084 tomba 8003B0E0 44
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 version: SLPM_80165 C of src/func_8011638C_slpm80165.c (func_8011638C_slpm80165, twin score 0.909) [-O2 -G0] (tools/demo_sweep.py)
// Demo-only helper: steps the frame (+0x2e) towards 131.
void func_8011638C_slpm80165(char *o)
{
    unsigned short x = *(unsigned short *)(o + 0x2e);
    if (x != 131) {
        if (x < 131) *(unsigned short *)(o + 0x2e) = x + 1;
        else *(unsigned short *)(o + 0x2e) = x - 1;
    }
}
#endif
