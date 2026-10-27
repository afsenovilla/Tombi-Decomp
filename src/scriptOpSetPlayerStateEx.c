// FUNC 8003a89c 416 MAIN0
// MATCHING 8003a89c 416
extern unsigned char DAT_8009c93a, DAT_8009c93e, DAT_8009c93f;
extern int DAT_8009d69c;
extern unsigned char *SCRIPT_CTX;
extern unsigned char PLAYER_STATE, PLAYER_SUBSTATE, PLAYER_STEP, PLAYER, DAT_800a60d6;
extern short DAT_800a60ea, DAT_800a6066, DAT_800a6058;
extern unsigned short GAME;
extern short DAT_800a6118;

void scriptOpSetPlayerStateEx(void)
{
    unsigned char *o = SCRIPT_CTX;
    int a, b;
    if (DAT_8009c93a == 0)
        return;
    if (DAT_8009d69c & 2) {
        PLAYER_STATE = *(int *)(o + 0x1190);
        b = *(int *)(o + 0x1194);
        PLAYER_STEP = 0;
        PLAYER_SUBSTATE = b;
        switch (PLAYER_STATE) {
        case 1:
            DAT_8009c93f = 0;
            PLAYER = 1;
            DAT_800a60d6 = 0;
            break;
        case 4:
            switch (PLAYER_SUBSTATE) {
            case 2:
                DAT_800a60ea = *(int *)(o + 0x11a0);
            case 1:
                DAT_800a6066 = *(int *)(o + 0x1198);
                DAT_800a6058 = *(int *)(o + 0x119c);
            }
            DAT_8009c93f = 1;
            break;
        }
    } else {
        a = *(int *)(o + 0x1190);
        b = *(int *)(o + 0x1194);
        if (a == 9) {
            if (b == 8)
                DAT_8009c93e = 0;
            if (b == 9)
                DAT_8009c93e = 1;
        } else {
            PLAYER_STATE = a;
            if (PLAYER_STATE == 5)
                DAT_8009c93f = 1;
            if (PLAYER_STATE == 1) {
                DAT_8009c93f = 0;
                if (GAME != 0 || DAT_800a6118 == 0)
                    PLAYER = 1;
            }
            PLAYER_SUBSTATE = b;
            PLAYER_STEP = 0;
        }
    }
    *(short *)(o + 0x8a) += 1;
}
