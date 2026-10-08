// FUNC 800f2a30 360 X000
extern unsigned char *DAT_8009c330;
extern unsigned char *DAT_800a611c;
extern unsigned char *DAT_8009d2e8;
extern unsigned char DAT_8009cf06, DAT_8009ce61, DAT_801152e8[];
extern void FUN_8001e5f4(int, int);
extern void FUN_8005a9a4(int, int);

void FUN_800f2a30(unsigned char *o)
{
    int v;
    char pad[8];
    unsigned short s;
    if (o[0xc9] != 0)
        *(short *)(DAT_8009c330 + 0x2e) = 0xff;
    o[0xa7] = 0;
    DAT_8009c330[8] = 0;
    FUN_8001e5f4(0x1c, 0x7f);
    o[0x9c] = 0;
    if (o[0xac] >= 2) {
        DAT_8009d2e8 = DAT_800a611c;
        DAT_800a611c[4] = 2;
        DAT_8009d2e8[5] = 2;
        DAT_8009d2e8[6] = 0;
    }
    v = *(short *)(o + 0x7c);
    o[0xac] = 0;
    if (v < 1) {
        *(short *)(o + 0x7c) = v + 0xd0;
        if ((v + 0xd0) * 0x10000 > 0)
            *(short *)(o + 0x7c) = 0;
    } else {
        *(short *)(o + 0x7c) = v - 0xd0;
        if ((v - 0xd0) * 0x10000 < 0)
            *(short *)(o + 0x7c) = 0;
    }
    s = *(unsigned short *)(o + 0x7c);
    *(short *)(o + 0x7c) = 0;
    *(short *)(o + 0x7e) = 0;
    *(unsigned short *)(o + 0xb2) = s;
    v = DAT_801152e8[*(short *)(o + 0xb0)];
    o[0xa5] = 0;
    *(int *)(o + 0x8c) = v;
    *(short *)(DAT_8009c330 + 0x20) = 0;
    if (DAT_8009cf06 != 0 && o[3] != 1) {
        o[3] = 1;
        if (DAT_8009ce61 != 0xff)
            FUN_8005a9a4(0xbd, 1);
    }
}
