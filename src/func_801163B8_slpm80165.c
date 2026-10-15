// VERSION SLPM_80165 x00 801163B8 44
#if defined(VERSION_SLPM_80165)
// Demo-only helper: advances the frame (+0x2e) by 2 up to 143.
void func_801163B8_slpm80165(char *o)
{
    *(unsigned short *)(o + 0x2e) += 2;
    if (*(unsigned short *)(o + 0x2e) >= 144)
        *(unsigned short *)(o + 0x2e) = 143;
}
#endif
