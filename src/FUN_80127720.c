// FUNC 80127720 256 X000
// MATCHING 80127720 256
extern unsigned short DAT_1f800172, DAT_1f80016e, DAT_1f80016a;
extern unsigned char *PTR_801390e0[];
extern unsigned FUN_8001f9e0(void);

unsigned char FUN_80127720(int a)
{
    short s;
    int d;
    short u;
    int v;
    unsigned char *t;
    if ((unsigned short)(*(unsigned short *)(*(int *)(a + 0x44) + 2) - DAT_1f800172 + 0x2d) >= 0x5b) return 0xff;
    if ((unsigned short)(*(unsigned short *)(a + 0x16) - DAT_1f80016e + 0x46) >= 0x6f) return 0xff;
    d = *(unsigned short *)(*(int *)(a + 0x40) + 2) - DAT_1f80016a;
    if ((unsigned short)(d + 0x80) >= 0x101) return 0xff;
                s = d;
                if ((short)d < 0)
                    s = -d;
                v = s >= 0x42;
                u = v;
                if (s >= 0x52)
                    u = v + 1;
                t = PTR_801390e0[u];
    return t[FUN_8001f9e0() & 0xf];
}
