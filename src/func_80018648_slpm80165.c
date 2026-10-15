// VERSION SLPM_80165 slpm80165 80018648 8
#if defined(VERSION_SLPM_80165)
// Demo-only helper: sets the halfword at +0x28.
void func_80018648_slpm80165(char *o, short v)
{
    *(short *)(o + 0x28) = v;
}
#endif
