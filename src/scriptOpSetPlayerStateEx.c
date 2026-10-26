// FUNC 8003a89c 416 MAIN0
// MATCHING 8003a89c 416
extern unsigned char DAT_8009c93a, DAT_8009c93e, DAT_8009c93f;
extern int DAT_8009d69c;
extern unsigned char *SCRIPT_CTX;
extern unsigned char DAT_800a603c, DAT_800a603d, DAT_800a603e, PLAYER, DAT_800a60d6;
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
        DAT_800a603c = *(int *)(o + 0x1190);
        b = *(int *)(o + 0x1194);
        DAT_800a603e = 0;
        DAT_800a603d = b;
        switch (DAT_800a603c) {
        case 1:
            DAT_8009c93f = 0;
            PLAYER = 1;
            DAT_800a60d6 = 0;
            break;
        case 4:
            switch (DAT_800a603d) {
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
            DAT_800a603c = a;
            if (DAT_800a603c == 5)
                DAT_8009c93f = 1;
            if (DAT_800a603c == 1) {
                DAT_8009c93f = 0;
                if (GAME != 0 || DAT_800a6118 == 0)
                    PLAYER = 1;
            }
            DAT_800a603d = b;
            DAT_800a603e = 0;
        }
    }
    *(short *)(o + 0x8a) += 1;
}
