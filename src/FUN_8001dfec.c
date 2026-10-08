// FUNC 8001dfec 512 MAIN0
// MATCHING 8001dfec 512
typedef struct { short a; short ch; } CH;
extern unsigned char *FUN_8001e1ec(int);
extern short DAT_800a3f90;
extern CH DAT_800a3d00[];
extern unsigned char DAT_8009f0d0[];
extern short DAT_800a3cc8[];
extern short DAT_8009c8c0[];
extern short DAT_1f8003a8[];
extern short DAT_800a34b0;
extern short DAT_8009d690;
extern int DAT_8009bd08;
extern short FUN_80070d44(int, int, int, int, int, int, int, int);

int FUN_8001dfec(int p)
{
    unsigned char *e;
    short *s;
    int ch;
    e = FUN_8001e1ec(p & 0x3ff);
    ch = DAT_800a3d00[DAT_800a3f90].ch;
    DAT_8009f0d0[ch] = 1;
    DAT_800a3cc8[ch] = -1;
    s = &DAT_8009c8c0[ch];
    if (*s < e[5])
        return -1;
    *s = e[5];
    switch (p & 0xc00) {
    case 0:
        DAT_8009bd08 = FUN_80070d44(ch, DAT_1f8003a8[e[0]], e[1], e[2], e[3], 0, e[4], e[4]);
        break;
    case 0x400:
        DAT_8009bd08 = FUN_80070d44(ch, DAT_1f8003a8[e[0]], e[1], e[2], DAT_800a34b0, 0, e[4], e[4]);
        break;
    case 0x800:
        DAT_8009bd08 = FUN_80070d44(ch, DAT_1f8003a8[e[0]], e[1], e[2], e[3], 0, DAT_8009d690, DAT_8009d690);
        break;
    case 0xc00:
        DAT_8009bd08 = FUN_80070d44(ch, DAT_1f8003a8[e[0]], e[1], e[2], DAT_800a34b0, 0, DAT_8009d690, DAT_8009d690);
        break;
    }
    return DAT_8009bd08;
}
