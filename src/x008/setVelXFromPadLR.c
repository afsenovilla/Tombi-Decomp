// FUNC 8011133c 128 X008
// MATCHING 8011133c 128
extern unsigned short PAD_INFO, PAD_PRESSED;
extern unsigned char DAT_8009d618;

void setVelXFromPadLR(char *o)
{
    short s;
    char *p;
    volatile unsigned short *k;
    p = o;
    s = 0x200;
    if (PAD_INFO == 7)
        s >>= 3 - DAT_8009d618;
    k = &PAD_PRESSED;
    if (*k & 0x80)
        *(short *)(o + 0x7c) = -s;
    else if (*k & 0x20)
        *(short *)(o + 0x7c) = s;
    else
        *(short *)(p + 0x7c) = 0;
}
