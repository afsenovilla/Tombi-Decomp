// FUNC 800ef490 84 X019
// MATCHING 800ef490 84
extern volatile unsigned short PAD_PRESSED;
void setFacingFromPad(char *o)
{
    unsigned short v = *(unsigned short *)(o + 0x2e) & 1;
    volatile unsigned short *p = &PAD_PRESSED;
    *(unsigned short *)(o + 0x2e) = v;
    if (*p & 0x20) {
        *(short *)(o + 0x2e) = 0;
    } else {
        *(unsigned short *)(o + 0x2e) = (*p & 0x80) ? 1 : (v | 2);
    }
}
