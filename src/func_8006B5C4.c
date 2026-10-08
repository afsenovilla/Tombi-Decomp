// FUNC 8006b5c4 104 MAIN0
// MATCHING 8006b5c4 104
typedef struct {
    int f0;
    int f4;
    int f8;
    char pad0c[0x14 - 0xc];
    int f14;
    int f18;
    char pad1c[0x46 - 0x1c];
    unsigned char f46;
    char pad47[2];
    unsigned char f49;
    char pad4a[0x5d - 0x4a];
    unsigned char f5d[6];
    char pad63[0xe3 - 0x63];
    unsigned char fe3;
    unsigned char fe4;
    char pade5;
    short fe6;
    char pade8;
    unsigned char fe9;
    unsigned char fea;
} S8006B5C4;

void func_8006B5C4(S8006B5C4 *a)
{
    int i;
    unsigned char *p;

    if (a->f49 != 0) {
        a->f49 = 0;
        a->f46 = 0;
        a->fe6 = 0;
        a->f14 = 0;
        a->f18 = 0;
        a->fe3 = 0;
        a->fe4 = 0;
        a->fe6 = 0;
        a->fe9 = 0;
        a->fea = 0;
        a->f0 = 0;
        a->f4 = 0;
        a->f8 = 0;
        p = a->f5d;
        for (i = 0; i < 6; i++) {
            *p++ = 0xff;
        }
    }
}
