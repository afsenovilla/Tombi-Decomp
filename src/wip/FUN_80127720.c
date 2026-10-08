// FUNC 80127720 256 X000
extern unsigned short DAT_1f800172, DAT_1f80016e, DAT_1f80016a;
extern unsigned char *PTR_801390e0[];
extern unsigned FUN_8001f9e0(void);

unsigned char FUN_80127720(int a)
{
    short s;
    int d;
    unsigned u, v;
    unsigned char *t;
    if ((unsigned short)(*(unsigned short *)(*(int *)(a + 0x44) + 2) - DAT_1f800172 + 0x2d) < 0x5b) {
            if ((unsigned short)(*(unsigned short *)(a + 0x16) - DAT_1f80016e + 0x46) < 0x6f) {
            d = *(unsigned short *)(*(int *)(a + 0x40) + 2) - DAT_1f80016a;
            s = d;
            if ((unsigned short)(d + 0x80) < 0x101) {
                if (d << 16 < 0)
                    s = -s;
                v = (s < 0x42) ^ 1;
                u = v;
                if (s > 0x51)
                    u = v + 1;
                t = PTR_801390e0[u];
                u = FUN_8001f9e0();
                return t[u & 0xf];
            } else
                    }
    }
    return r;
}
