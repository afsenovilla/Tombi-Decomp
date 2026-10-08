// FUNC 80110fdc 400 X000
extern unsigned char DAT_8009d2b3, DAT_8009cf06, DAT_8009d006;
extern unsigned short DAT_8009c990;
extern unsigned char DAT_80115510[];
#define TS(i, o) (*(short *)(DAT_80115510 + (i) + (o)))

void FUN_80110fdc(unsigned char *p)
{
    unsigned u;
    short s, s1;
    int i, n, c;
    c = p[0xc1];
    u = DAT_8009d2b3 + c * 4;
    if ((DAT_8009c990 & 3) != 0)
        u = c << 2 | 3;
    if (DAT_8009cf06 != 0)
        u = 8;
    if (DAT_8009d006 != 0)
        u = 8;
    s = *(short *)(p + 0xb2);
    if ((unsigned short)(s + 0x50) < 0xa1) {
        *(short *)(p + 0xb2) = 0;
    } else {
        i = u * 0x74;
        n = s;
        if (TS(i, 2) < n) {
            s = s - TS(i, 0x66);
        } else if (TS(i, 0) < n) {
            s = s - TS(i, 0x68);
        } else {
            if (n < 1) {
                if (n < -(int)TS(i, 2)) {
                    s1 = TS(i, 0x66);
                } else {
                    if (-(int)TS(i, 0) <= n) {
                        if (-1 < n)
                            return;
                        *(short *)(p + 0xb2) = s + TS(i, 0x6a);
                        return;
                    }
                    s1 = TS(i, 0x68);
                }
                *(short *)(p + 0xb2) = s + s1;
                return;
            }
            s = s - TS(i, 0x6a);
        }
        *(short *)(p + 0xb2) = s;
    }
}
