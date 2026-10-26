// FUNC 800eea7c 104 X013
// MATCHING 800eea7c 104
typedef struct D { char pad[0x2c]; unsigned short a; unsigned short b; } D;
extern D *PLAYER_EXT;
extern void FUN_800efc04(int);
extern void FUN_8001fe94(int, int);
void playerSetAnim(int o, short a, short c)
{
    PLAYER_EXT->a = a;
    FUN_800efc04(o);
    FUN_8001fe94(o, c);
    PLAYER_EXT->b = PLAYER_EXT->a;
}
