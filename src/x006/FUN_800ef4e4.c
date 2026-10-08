// FUNC 800ef4e4 164 X006
// MATCHING 800ef4e4 164
extern volatile unsigned short DAT_8009d670;

void FUN_800ef4e4(char *o)
{
    unsigned short v = *(unsigned short *)(o + 0x2e) & 1;
    volatile unsigned short *p = &DAT_8009d670;
    volatile unsigned short *q;
    *(unsigned short *)(o + 0x2e) = v;
    if (*p & 0x20) {
        *(short *)(o + 0x2e) = 0;
    } else {
        *(unsigned short *)(o + 0x2e) = (*p & 0x80) ? 1 : (v | 2);
    }
    q = &DAT_8009d670;
    if (*q & 0x10)
        *(unsigned short *)(o + 0x2e) |= 8;
    if (*q & 0x40)
        *(unsigned short *)(o + 0x2e) |= 4;
}
