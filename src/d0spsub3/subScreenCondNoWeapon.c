// FUNC 800ee94c 16 D0SPSUB3
// MATCHING 800ee94c 16
extern unsigned char GAME_BITTEN_JUMP_KIND;

int subScreenCondNoWeapon(void)
{
    return GAME_BITTEN_JUMP_KIND == 0;
}
