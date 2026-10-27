// FUNC 800e8a2c 48 O0PTSUB0
// MATCHING 800e8a2c 48
extern char BOOT_LOGO_SPRITE_RECS[];
extern void titleDrawSpriteRecs(void *list, int x, int y, int n);

void bootLogoDraw(void)
{
    titleDrawSpriteRecs(BOOT_LOGO_SPRITE_RECS, 0, 0, 1);
}
