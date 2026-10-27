// FUNC 800e8a14 24 O0PTSUB0
// MATCHING 800e8a14 24
extern char BOOT_LOGO_SPRITE_DATA[];
extern char *SEQ_DATA;

void bootLogoSetSpriteBase(void)
{
    SEQ_DATA = BOOT_LOGO_SPRITE_DATA;
}
