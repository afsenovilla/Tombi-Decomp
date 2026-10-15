// VERSION SLPM_80165 x00 800EB67C 56
#if defined(VERSION_SLPM_80165)
// Demo-only helper (copy of func_800EB644): clears the hit box and rotation, sort key 17, next state.
void func_800EB67C_slpm80165(unsigned char *o)
{
    *(short *)(o + 0x6c) = 0;
    *(short *)(o + 0x6e) = 0;
    *(short *)(o + 0x70) = 0;
    *(short *)(o + 0x72) = 0;
    *(int *)(o + 0x84) = 0;
    *(int *)(o + 0x88) = 0;
    *(int *)(o + 0x8c) = 0;
    o[0xa] = 17;
    o[6] = 0;
    o[4]++;
}
#endif
