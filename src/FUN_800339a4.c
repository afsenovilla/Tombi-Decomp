// FUNC 800339a4 416 MAIN0
// MATCHING 800339a4 416
extern unsigned char DAT_800a60d6;
extern unsigned char DAT_800a60d4;
extern unsigned char DAT_800a603d;
extern unsigned char DAT_800a603e;
extern unsigned char DAT_800a60fe;
extern int DAT_800a60c4;
extern unsigned char *DAT_8009c330;

void FUN_800339a4(unsigned char *o)
{
    short flag;
    if (DAT_800a60d6 == 4 || DAT_800a60d6 == 7) {
        if (((signed char *)DAT_8009c330)[4] == 0) {
            DAT_800a60fe = 1;
            DAT_8009c330[0] = 2;
            o[5] = 1;
            DAT_800a603d = 0x18;
            DAT_800a603e = 1;
            return;
        }
    } else {
        flag = 0;
        switch (DAT_800a603d) {
        case 5: case 6: case 7: case 0xb: case 0xc: case 0xd: case 0xe: case 0xf:
        case 0x10: case 0x11: case 0x12: case 0x15: case 0x16: case 0x17: case 0x1b:
        case 0x1d: case 0x1e: case 0x20: case 0x21: case 0x23: case 0x24: case 0x25:
        case 0x26: case 0x27: case 0x28: case 0x29: case 0x2a: case 0x2b: case 0x2e:
        case 0x2f: case 0x30: case 0x33: case 0x34: case 0x35: case 0x36: case 0x38:
        case 0x39: case 0x3a: case 0x3b: case 0x3c: case 0x45:
            flag++;
        }
        if (flag == 0) {
            o[5] = 0;
            DAT_800a60fe = 1;
            DAT_8009c330[0] = 2;
            switch (DAT_800a60d4) {
            case 0:
                DAT_800a603d = 3;
                DAT_800a603e = 1;
                break;
            case 1:
                DAT_800a60c4 = 0;
                DAT_800a603d = 4;
                if (DAT_8009c330[8] != 0)
                    DAT_800a603e = 2;
                else
                    DAT_800a603e = 1;
                break;
            case 2:
                DAT_800a603d = 4;
                DAT_800a60c4 = 0;
                DAT_800a603e = 3;
                break;
            }
            return;
        }
    }
    o[4] = 2;
    o[5] = 0;
    o[6] = 0;
}
