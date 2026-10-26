// FUNC 800ef150 92 X006
// MATCHING 800ef150 92
extern void FUN_8001e5f4(int, int);
extern char *PLAYER_EXT;
void playerResetStep(char *o)
{
    FUN_8001e5f4(0x1c, 0x7f);
    PLAYER_EXT[8] = 0;
    o[0xa7] = 0;
    o[0xac] = 0;
    o[0x9c] = 0;
    o[0x9e] = 0;
    o[0xaa] = 0;
    o[5] = 0;
    o[6] = 0;
}
