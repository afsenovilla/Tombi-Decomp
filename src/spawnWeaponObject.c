// FUNC 800ee948 132 X000
// MATCHING 800ee948 132
// VERSION SLPS_01144 x00 800EEB04 132
#if defined(VERSION_SLPS_01144)
// Japanese version of PAL x000 800EE948 (spawnWeaponObject.c): 227 -> 211.
extern unsigned char GAME_EQUIPPED_WEAPON[];
extern short DAT_8007a038[];
extern unsigned char *allocObjectLayer1(void);
void spawnWeaponObject(signed char *o, unsigned char p)
{
    unsigned char *q;
    int pad[1];
    if (o[0xd3] < DAT_8007a038[GAME_EQUIPPED_WEAPON[0]]) {
        o[0xd3]++;
        q = allocObjectLayer1();
        if (q != 0) {
            q[0] = 1;
            q[2] = GAME_EQUIPPED_WEAPON[0];
            q[5] = p;
            q[6] = 0;
        }
    }
}
#else
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
#endif
