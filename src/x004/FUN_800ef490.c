// FUNC 800ef490 84 X004
// MATCHING 800ef490 84
extern volatile unsigned short DAT_8009d670;
void FUN_800ef490(char *o)
{
    unsigned short v = *(unsigned short *)(o + 0x2e) & 1;
    volatile unsigned short *p = &DAT_8009d670;
    *(unsigned short *)(o + 0x2e) = v;
    if (*p & 0x20) {
        *(short *)(o + 0x2e) = 0;
    } else {
        *(unsigned short *)(o + 0x2e) = (*p & 0x80) ? 1 : (v | 2);
    }
}
