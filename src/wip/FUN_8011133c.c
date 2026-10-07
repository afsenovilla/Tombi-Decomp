// FUNC 8011133c 128 X000
extern unsigned short DAT_8009d610, DAT_8009d670;
extern unsigned char DAT_8009d618;

void FUN_8011133c(char *o)
{
    char *p = o;
    short s = 0x200;
    if (DAT_8009d610 == 7)
        s = 0x200 >> (3 - DAT_8009d618);
    if ((DAT_8009d670 & 0x80) == 0) {
        if ((DAT_8009d670 & 0x20) == 0) {
            char *q = o;
            *(short *)(q + 0x7c) = 0;
        } else
            *(short *)(p + 0x7c) = s;
    } else {
        *(short *)(p + 0x7c) = -s;
    }
}
