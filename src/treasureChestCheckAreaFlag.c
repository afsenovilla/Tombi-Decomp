// FUNC 80112be0 68 X000
// MATCHING 80112be0 68
extern unsigned char DAT_8009c964;
extern unsigned char GVARS_PLAYER_LANDED;
extern void fa(void);

void treasureChestCheckAreaFlag(void)
{
    if (DAT_8009c964 != 0x21 || GVARS_PLAYER_LANDED != 1) fa();
}
