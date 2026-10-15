// JPDEMO x00 801163E4 44
// Demo-only helper: advances the frame (+0x2e) by 2 up to 135.
void func_801163E4(char *o)
{
    *(unsigned short *)(o + 0x2e) += 2;
    if (*(unsigned short *)(o + 0x2e) >= 136)
        *(unsigned short *)(o + 0x2e) = 135;
}
