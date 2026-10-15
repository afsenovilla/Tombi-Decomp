// VERSION SLPM_80165 slpm80165 80020570 12
#if defined(VERSION_SLPM_80165)
// Demo-only helper: copies the timer (+0x20) to +0x2c.
void func_80020570_slpm80165(char *o)
{
    *(short *)(o + 0x2c) = *(short *)(o + 0x20);
}
#endif
