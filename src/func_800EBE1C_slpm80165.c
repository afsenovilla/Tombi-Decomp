// VERSION SLPM_80165 x00 800EBE1C 80
#if defined(VERSION_SLPM_80165)
// Demo-only helper: unless +0xad is set, raises y by 3 (small +0xb0) or 5 pixels.
void func_800EBE1C_slpm80165(unsigned char *o)
{
    if (o[0xad] == 0) {
        if ((unsigned short)(*(unsigned short *)(o + 0xb0) + 5) >= 11)
            *(int *)(o + 0x14) += 0x50000;
        else
            *(int *)(o + 0x14) += 0x30000;
    }
}
#endif
