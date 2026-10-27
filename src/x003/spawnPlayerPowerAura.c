// FUNC 80111a50 144 X003
// MATCHING 80111a50 144
extern int PLAYER_POS, PLAYER_Y_FIX, DAT_800a6050;
extern unsigned char *allocObjectLayer1(void);
void spawnPlayerPowerAura(unsigned char a, unsigned char b, unsigned char c)
{
    unsigned char *q = allocObjectLayer1();
    if (q != 0) {
        q[0] = 1;
        q[2] = 10;
        q[3] = a;
        q[0xc] = b;
        *(int *)(q + 0x10) = PLAYER_POS;
        *(int *)(q + 0x14) = PLAYER_Y_FIX;
        *(int *)(q + 0x18) = DAT_800a6050;
        q[5] = c;
    }
}
