// FUNC 800fb280 240 X000
extern short DAT_800a60ea;
extern short DAT_800a60ec;
extern char *DAT_8009c330;
int FUN_800fb280(void)
{
    int r = 0;
    switch (DAT_800a60ea) {
    case 1:
        if (DAT_800a60ec < 0x51 && DAT_800a60ec < 0x29) {
            if (DAT_800a60ec < 1) break;
            r = 3;
            break;
        }
        r = 2;
        break;
    case 2:
    case 3:
        if (DAT_800a60ec < 0x51) {
            if (DAT_800a60ec < 0x29) {
                if (DAT_800a60ec < 1) break;
            }
            r = 3;
            break;
        }
        r = 2;
        break;
    case 4:
        if (DAT_800a60ec < 0x51) {
            if (DAT_800a60ec < 0x29) {
                if (DAT_800a60ec < 1) break;
            }
            r = 3;
            break;
        }
        r = 2;
        break;
    }
    if (DAT_8009c330[10] != 0) r = 4;
    return r;
}
