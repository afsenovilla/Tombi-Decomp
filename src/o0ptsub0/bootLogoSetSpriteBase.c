// FUNC 800e8a14 24 O0PTSUB0
// MATCHING 800e8a14 24
extern char D_800EB014[];
extern char *D_8009D630;

void bootLogoSetSpriteBase(void)
{
    D_8009D630 = D_800EB014;
}
