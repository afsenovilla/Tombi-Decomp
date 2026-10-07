// FUNC 800ef6d8 136 X000
extern volatile unsigned short DAT_8009d670;

void FUN_800ef6d8(char *o)
{
    unsigned short v;
    if ((*(unsigned short *)(o + 0x2e) & 1) == 0) {
        v = 2;
        if ((DAT_8009d670 & 0x10) != 0) {
            v = 4;
            if ((DAT_8009d670 & 0x20) == 0)
                v = 6;
        }
    } else {
        v = 3;
        if ((DAT_8009d670 & 0x10) != 0) {
            v = 5;
            if ((DAT_8009d670 & 0x80) == 0)
                v = 7;
        }
    }
    *(unsigned short *)(o + 0x2e) = v;
}
