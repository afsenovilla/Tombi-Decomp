// FUNC 80118e70 288 X000
extern unsigned short DAT_8009c960;
extern unsigned short DAT_1f800250;
extern short DAT_1f80019c;
extern unsigned char **DAT_1f800260;

void FUN_80118e70(unsigned char *o)
{
    short n;
    unsigned int d;
    unsigned char **pp;
    unsigned char *e;

    if (DAT_8009c960 != 0)
        return;
    DAT_1f80019c = DAT_1f800250;
    pp = DAT_1f800260;
    if (DAT_1f800250 == 0)
        return;
    do {
        e = *pp++;
        n = DAT_1f80019c - 1;
        DAT_1f80019c = n;
        if ((*(unsigned int *)e & 0xffff0000) == 0x1020000) {
            if ((*e & 1) == 0) {
                return;
            }
            if (e[4] != 1) {
                return;
            }
            d = (*(unsigned short *)(o + 0x12) - *(unsigned short *)(e + 0x12)) + 0x40;
            if (*(short *)(e + 0x6e) + 0x60 <
                (int)((*(unsigned short *)(e + 0x6c) + d) & 0xffff)) {
                return;
            }
            d = (*(unsigned short *)(o + 0x16) - *(unsigned short *)(e + 0x16)) + 0x2c;
            if (*(short *)(e + 0x72) + 0x18 <
                (int)((*(unsigned short *)(e + 0x70) + d) & 0xffff)) {
                return;
            }
            *e = 2;
            e[4] = 2;
            e[5] = 3;
            e[6] = 0;
            return;
        }
    } while (n != 0);
}
