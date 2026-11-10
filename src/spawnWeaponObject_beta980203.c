// VERSION BETA_980203 x00 800F3470 132
#if defined(VERSION_BETA_980203)
// Beta version of src/spawnWeaponObject.c (spawnWeaponObject, PAL body): PLAYER field 0xe3 is at 0xd7.
extern unsigned char GAME_EQUIPPED_WEAPON[];
extern short DAT_8007a038[];
extern unsigned char *allocObjectLayer1(void);
void spawnWeaponObject(signed char *o, unsigned char p)
{
    unsigned char *q;
    int pad[1];
    if (o[0xd7] < DAT_8007a038[GAME_EQUIPPED_WEAPON[0]]) {
        o[0xd7]++;
        q = allocObjectLayer1();
        if (q != 0) {
            q[0] = 1;
            q[2] = GAME_EQUIPPED_WEAPON[0];
            q[5] = p;
            q[6] = 0;
        }
    }
}
#endif
