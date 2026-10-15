// JPDEMO x00 800EBE1C 80
// Demo-only helper: unless +0xad is set, raises y by 3 (small +0xb0) or 5 pixels.
void func_800EBE1C(unsigned char *o)
{
    if (o[0xad] == 0) {
        if ((unsigned short)(*(unsigned short *)(o + 0xb0) + 5) >= 11)
            *(int *)(o + 0x14) += 0x50000;
        else
            *(int *)(o + 0x14) += 0x30000;
    }
}
