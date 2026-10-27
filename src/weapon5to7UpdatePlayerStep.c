// FUNC 800339a4 416 MAIN0
// MATCHING 800339a4 416
extern unsigned char DAT_800a60d6;
extern unsigned char DAT_800a60d4;
extern unsigned char PLAYER_SUBSTATE;
extern unsigned char PLAYER_STEP;
extern unsigned char DAT_800a60fe;
extern int DAT_800a60c4;
extern unsigned char *PLAYER_EXT;

void weapon5to7UpdatePlayerStep(unsigned char *o)
{
    short flag;
    if (DAT_800a60d6 == 4 || DAT_800a60d6 == 7) {
        if (((signed char *)PLAYER_EXT)[4] == 0) {
            DAT_800a60fe = 1;
            PLAYER_EXT[0] = 2;
            o[5] = 1;
            PLAYER_SUBSTATE = 0x18;
            PLAYER_STEP = 1;
            return;
        }
    } else {
        flag = 0;
        switch (PLAYER_SUBSTATE) {
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
            PLAYER_EXT[0] = 2;
            switch (DAT_800a60d4) {
            case 0:
                PLAYER_SUBSTATE = 3;
                PLAYER_STEP = 1;
                break;
            case 1:
                DAT_800a60c4 = 0;
                PLAYER_SUBSTATE = 4;
                if (PLAYER_EXT[8] != 0)
                    PLAYER_STEP = 2;
                else
                    PLAYER_STEP = 1;
                break;
            case 2:
                PLAYER_SUBSTATE = 4;
                DAT_800a60c4 = 0;
                PLAYER_STEP = 3;
                break;
            }
            return;
        }
    }
    o[4] = 2;
    o[5] = 0;
    o[6] = 0;
}
