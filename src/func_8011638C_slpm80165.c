// VERSION SLPM_80165 x00 8011638C 44
#if defined(VERSION_SLPM_80165)
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
