// VERSION SLPM_80165 x00 80116340 32
#if defined(VERSION_SLPM_80165)
// Demo-only helper: counts +0x2e up to 143.
void func_80116340_slpm80165(char *o)
{
    if (*(unsigned short *)(o + 0x2e) != 143)
        (*(unsigned short *)(o + 0x2e))++;
}
#endif
