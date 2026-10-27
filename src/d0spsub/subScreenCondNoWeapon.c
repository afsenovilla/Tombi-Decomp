// FUNC 800f182c 16 D0SPSUB
// MATCHING 800f182c 16
extern unsigned char GAME_BITTEN_JUMP_KIND;

int subScreenCondNoWeapon(void)
{
    return GAME_BITTEN_JUMP_KIND == 0;
}
