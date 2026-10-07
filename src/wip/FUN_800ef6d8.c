// FUNC 800ef6d8 136 X000
extern unsigned short DAT_8009d670;

void FUN_800ef6d8(char *o)
{
    int v;
    if (*(unsigned short *)(o + 0x2e) & 1) {
        register volatile unsigned short *pad = &DAT_8009d670;
        v = 3;
        if ((*pad & 0x10) != 0) {
            v = 5;
            if ((*pad & 0x80) == 0)
                v = 7;
        }
    } else {
        register volatile unsigned short *pad = &DAT_8009d670;
        v = 2;
        if ((*pad & 0x10) != 0) {
            v = 4;
            if ((*pad & 0x20) == 0)
                v = 6;
        }
    }
    *(unsigned short *)(o + 0x2e) = v;
}
