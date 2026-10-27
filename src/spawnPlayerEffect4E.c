// FUNC 8002ff20 144 MAIN0
// MATCHING 8002ff20 144
extern char *FUN_80018448();
extern int PLAYER_POS, PLAYER_Y_FIX, DAT_800a6050;

void spawnPlayerEffect4E(char a, char b, char c)
{
    int t;
    char *p = FUN_80018448();
    if (p != 0) {
        p[0] = 1;
        p[2] = 0x4e;
        p[3] = a;
        p[0xc] = b;
        *(int *)(p + 0x10) = PLAYER_POS;
        *(int *)(p + 0x14) = PLAYER_Y_FIX;
        t = DAT_800a6050;
        p[5] = c;
        *(int *)(p + 0x18) = t;
    }
}
