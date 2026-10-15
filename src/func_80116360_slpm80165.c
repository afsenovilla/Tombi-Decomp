// VERSION SLPM_80165 x00 80116360 44
#if defined(VERSION_SLPM_80165)
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
