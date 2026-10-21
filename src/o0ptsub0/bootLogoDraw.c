// FUNC 800e8a2c 48 O0PTSUB0
// MATCHING 800e8a2c 48
extern char D_800EB13C[];
extern void titleDrawSpriteRecs(void *list, int x, int y, int n);

void bootLogoDraw(void)
{
    titleDrawSpriteRecs(D_800EB13C, 0, 0, 1);
}
