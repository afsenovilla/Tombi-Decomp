// FUNC 8002c654 232 MAIN0
extern unsigned char DAT_8009cef8;
extern unsigned char *ObjAlloc(void);
extern unsigned short GetClut(int, int);
extern unsigned short DAT_800a6066;
extern int DAT_800a6048, DAT_800a604c, DAT_800a6050;
extern unsigned char DAT_800a6047;
extern int DAT_1f8002cc;

void FUN_8002c654(void)
{
    unsigned char *o;

    if (DAT_8009cef8 != 0 && (o = ObjAlloc()) != 0) {
        int v, w;
        o[0] = 1;
        o[2] = 0x12;
        *(unsigned short *)(o + 0x2e) = DAT_800a6066 & 1;
        *(int *)(o + 0x10) = DAT_800a6048;
        v = DAT_800a604c;
        w = DAT_800a6050;
        *(int *)(o + 0x14) = v;
        *(short *)(o + 0x1e) = 0;
        *(int *)(o + 0x18) = w;
        *(unsigned short *)(o + 8) = GetClut(0x80, 499);
        o[0xd] = 1;
        o[0xa] = 8;
        o[3] = 1;
        o[0xf] = DAT_800a6047 + 1;
        v = DAT_1f8002cc;
        o[0x1c] = o[0x1c] | 0x80;
        o[0x1d] = 0x4d;
        *(int *)(o + 0x3c) = v;
    }
}
