// JP x00 800EF650 164
// Japanese version of PAL x000 800EF4E4 (setFacingAndUpDownFromPad.c): 64 -> 16384, 128 -> 32768, 16 -> 4096, 32 -> 8192.
extern volatile unsigned short DAT_8009d670;

void setFacingAndUpDownFromPad(char *o)
{
    unsigned short v = *(unsigned short *)(o + 0x2e) & 1;
    volatile unsigned short *p = &DAT_8009d670;
    volatile unsigned short *q;
    *(unsigned short *)(o + 0x2e) = v;
    if (*p & 0x2000) {
        *(short *)(o + 0x2e) = 0;
    } else {
        *(unsigned short *)(o + 0x2e) = (*p & 0x8000) ? 1 : (v | 2);
    }
    q = &DAT_8009d670;
    if (*q & 0x1000)
        *(unsigned short *)(o + 0x2e) |= 8;
    if (*q & 0x4000)
        *(unsigned short *)(o + 0x2e) |= 4;
}
