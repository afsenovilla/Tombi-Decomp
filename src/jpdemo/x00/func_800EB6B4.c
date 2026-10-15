// JPDEMO x00 800EB6B4 80
// Demo-only helper: clears the hit box and rotation (+0x88 = 0x800 when facing 2), sort key 17, next state.
void func_800EB6B4(unsigned char *o)
{
    *(short *)(o + 0x6c) = 0;
    *(short *)(o + 0x6e) = 0;
    *(short *)(o + 0x70) = 0;
    *(short *)(o + 0x72) = 0;
    *(int *)(o + 0x84) = 0;
    if (*(unsigned short *)(o + 0x2e) == 2)
        *(int *)(o + 0x88) = 0x800;
    else
        *(int *)(o + 0x88) = 0;
    *(int *)(o + 0x8c) = 0;
    o[0xa] = 17;
    o[6] = 0;
    o[4]++;
}
