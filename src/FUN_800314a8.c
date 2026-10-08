// FUNC 800314a8 224 MAIN0
// MATCHING 800314a8 224
extern char DAT_80010f10[], DAT_80010f30[], DAT_80010f50[], DAT_80010e80[], DAT_80010ea0[], DAT_80010ec0[];
extern char *DAT_800a605c;
void FUN_800314a8(char *o)
{
    int s = (unsigned char)o[5];
    if (s != 1) {
        if (s < 2) {
            if (s == 0) {
        switch (*(unsigned short *)(o + 0x2e)) {
        case 0: case 1: case 2: case 3:
            DAT_800a605c = DAT_80010e80;
            break;
        case 4: case 5:
            DAT_800a605c = DAT_80010ea0;
            break;
        case 6: case 7:
            DAT_800a605c = DAT_80010ec0;
        }
            }
        }
    } else {
        switch (*(unsigned short *)(o + 0x2e)) {
        case 0: case 1: case 2: case 3:
            DAT_800a605c = DAT_80010f10;
            break;
        case 4: case 5:
            DAT_800a605c = DAT_80010f30;
            break;
        case 6: case 7:
            DAT_800a605c = DAT_80010f50;
        }
    }
}
