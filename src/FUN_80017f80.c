// FUNC 80017f80 108 MAIN0
// MATCHING 80017f80 108
extern char DAT_800b1320[];
extern char *DAT_1f800220, *DAT_1f800264;
extern short DAT_1f800252, DAT_1f80024a;
extern char DAT_800a3fe0[];
extern void FUN_80018de0(void);

void FUN_80017f80(void)
{
    int i = 7;
    int m = -1;
    int off = 0x3d4;
    DAT_1f800220 = DAT_800b1320;
    DAT_1f800264 = DAT_800b1320;
    DAT_1f800252 = 0;
    DAT_1f80024a = 0;
    do {
        *(short *)(DAT_800a3fe0 + off) = m;
        i--;
        off -= 0x8c;
    } while (i >= 0);
    FUN_80018de0();
}
