// FUNC 800fb280 240 X001
// MATCHING 800fb280 240
extern short DAT_800a60ea;
extern short DAT_800a60ec;
extern char *DAT_8009c330;
int FUN_800fb280(void)
{
    int r = 0;
    switch (DAT_800a60ea) {
    case 1:
        if (DAT_800a60ec > 80) r = 2;
        else if (DAT_800a60ec > 40) r = 2;
        else if (DAT_800a60ec > 0) r = 3;
        break;
    case 2:
        if (DAT_800a60ec > 80) r = 2;
        else if (DAT_800a60ec > 40) r = 3;
        else if (DAT_800a60ec > 0) r = 3;
        break;
    case 3:
        if (DAT_800a60ec > 80) r = 2;
        else if (DAT_800a60ec > 40) r = 3;
        else if (DAT_800a60ec > 0) r = 3;
        break;
    case 4:
        if (DAT_800a60ec > 80) r = 2;
        else if (DAT_800a60ec > 40) r = 3;
        else if (DAT_800a60ec > 0) r = 3;
        break;
    }
    if (DAT_8009c330[10] != 0) r = 4;
    return r;
}
