// JP x00 800EF5FC 84
// Japanese version of PAL x000 800EF490 (setFacingFromPad.c): 128 -> 32768, 32 -> 8192.
extern volatile unsigned short DAT_8009d670;
void setFacingFromPad(char *o)
{
    unsigned short v = *(unsigned short *)(o + 0x2e) & 1;
    volatile unsigned short *p = &DAT_8009d670;
    *(unsigned short *)(o + 0x2e) = v;
    if (*p & 0x2000) {
        *(short *)(o + 0x2e) = 0;
    } else {
        *(unsigned short *)(o + 0x2e) = (*p & 0x8000) ? 1 : (v | 2);
    }
}
