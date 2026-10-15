// VERSION SLPM_80165 x00 800EB5FC 40
#if defined(VERSION_SLPM_80165)
// Demo-only helper: in section 2, sets +0x88 to 0x600 and state 2.
extern unsigned short D_8009A812;
void func_800EB5FC_slpm80165(char *o)
{
    if (D_8009A812 == 2) {
        *(int *)(o + 0x88) = 0x600;
        o[6] = 2;
    }
}
#endif
