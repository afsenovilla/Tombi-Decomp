// VERSION SLPM_80165 slpm80165 80049420 16
#if defined(VERSION_SLPM_80165)
// Demo-only helper: sets the halfwords at +0xa/+0xc/+0xe.
void func_80049420_slpm80165(char *o, short a, short b, short c)
{
    *(short *)(o + 0xa) = a;
    *(short *)(o + 0xc) = b;
    *(short *)(o + 0xe) = c;
}
#endif
