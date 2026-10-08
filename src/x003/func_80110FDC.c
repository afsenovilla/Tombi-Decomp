// FUNC 80110fdc 400 X003
// MATCHING 80110fdc 400
extern unsigned char DAT_8009d2b3, DAT_8009cf06, DAT_8009d006;
extern unsigned char DAT_8009c990;
extern unsigned char DAT_80115510[];
#define TS(i, o) (*(short *)(DAT_80115510 + (i) + (o)))

void func_80110FDC(unsigned char *p)
{
    unsigned u;
    unsigned short s;
    int i, n, c;
    unsigned char *q = p;
    c = p[0xc1];
    u = DAT_8009d2b3 + c * 4; if ((DAT_8009c990 & 3) != 0) u = (unsigned char)c << 2 | 3;
    if (DAT_8009cf06 != 0)
        u = 8;
    if (DAT_8009d006 != 0)
        u = 8;
    s = *(unsigned short *)(p + 0xb2);
    if ((unsigned short)(s + 0x50) < 0xa1) {
        *(short *)(p + 0xb2) = 0;
        return;
    }
    i = u * 0x74;
    n = (short)s;
    if (TS(i, 2) < n)
        *(short *)(p + 0xb2) = s - TS(i, 0x66);
    else if (TS(i, 0) < n)
        *(short *)(p + 0xb2) = s - TS(i, 0x68);
    else if (n > 0)
        *(short *)(p + 0xb2) = s - TS(i, 0x6a);
    else if (n < -TS(i, 2))
        *(short *)(p + 0xb2) = s + TS(i, 0x66);
    else if (n < -TS(i, 0))
        *(short *)(p + 0xb2) = s + TS(i, 0x68);
    else if (n < 0)
        *(short *)(q + 0xb2) = s + TS(i, 0x6a);
}
