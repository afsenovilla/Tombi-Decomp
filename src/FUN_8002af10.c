// FUNC 8002af10 264 MAIN0
// MATCHING 8002af10 264
extern unsigned short DAT_8009c960;
extern unsigned short DAT_8009c962;
extern unsigned char DAT_800b0d9b;
extern unsigned char DAT_800b0d99;
extern unsigned char DAT_8009cda2;
extern char DAT_800b0d98[];
extern int DAT_1f800330;
extern void (*PTR_DAT_80079b3c[])(char *, int);

void FUN_8002af10(void)
{
    unsigned short v, x;
    DAT_800b0d9b = 0;
    switch (DAT_8009c960) {
    case 0:
        break;
    case 0xd:
        if (DAT_8009c962 != 0) return;
        break;
    case 2:
        x = DAT_8009c962;
        v = 3;
        goto cmp;
    case 6:
        if (DAT_8009c962 > 1) return;
        break;
    case 9:
        x = DAT_8009c962;
        if (x == 0) break;
        v = 6;
        goto cmp;
    case 0x12:
        x = DAT_8009c962;
        v = 1;
    cmp:
        if (x != v) return;
        break;
    case 5: case 8: case 0xb: case 0x10: case 0x11: case 0x13:
        return;
    }
    PTR_DAT_80079b3c[DAT_8009c960](DAT_800b0d98, DAT_1f800330);
    DAT_800b0d99 = DAT_8009cda2;
}
