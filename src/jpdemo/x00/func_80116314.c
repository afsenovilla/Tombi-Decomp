// JPDEMO x00 80116314 44
// Demo-only helper: steps the frame (+0x2e) towards 128.
void func_80116314(char *o)
{
    unsigned short x = *(unsigned short *)(o + 0x2e);
    if (x != 128) {
        if (x <= 128) *(unsigned short *)(o + 0x2e) = x + 1;
        else *(unsigned short *)(o + 0x2e) = x - 1;
    }
}
