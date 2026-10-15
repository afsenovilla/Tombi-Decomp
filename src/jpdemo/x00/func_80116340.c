// JPDEMO x00 80116340 32
// Demo-only helper: counts +0x2e up to 143.
void func_80116340(char *o)
{
    if (*(unsigned short *)(o + 0x2e) != 143)
        (*(unsigned short *)(o + 0x2e))++;
}
