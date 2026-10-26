// FUNC 800fb370 164 X019
// MATCHING 800fb370 164
typedef struct P { char pad[0x2c]; unsigned short w2c; unsigned short w2e; } P;
extern P *PLAYER_EXT;
extern void FUN_800efc04();
extern void FUN_8001fe94();
void playerRopeSetAnim(char *o)
{
    if (*(short *)(o + 0xb4) > 0x28) {
        unsigned u = (*(int *)(o + 0x84) >> 3) & 0xf;
        PLAYER_EXT->w2c = 0x31;
        FUN_800efc04(o);
        FUN_8001fe94(o, u);
    } else {
        PLAYER_EXT->w2c = 0x31;
        FUN_800efc04(o);
        FUN_8001fe94(o, 0xf);
    }
    PLAYER_EXT->w2e = PLAYER_EXT->w2c;
}
