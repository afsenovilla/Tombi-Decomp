// FUNC 800ee948 132 X010
// MATCHING 800ee948 132
extern unsigned char GAME_EQUIPPED_WEAPON[];
extern short DAT_8007a038[];
extern unsigned char *allocObjectLayer1(void);
void spawnWeaponObject(signed char *o, unsigned char p)
{
    unsigned char *q;
    int pad[1];
    if (o[0xe3] < DAT_8007a038[GAME_EQUIPPED_WEAPON[0]]) {
        o[0xe3]++;
        q = allocObjectLayer1();
        if (q != 0) {
            q[0] = 1;
            q[2] = GAME_EQUIPPED_WEAPON[0];
            q[5] = p;
            q[6] = 0;
        }
    }
}
