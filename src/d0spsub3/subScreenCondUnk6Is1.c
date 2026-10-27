// FUNC 800ee658 24 D0SPSUB3
// MATCHING 800ee658 24
extern unsigned char GAME_AREA00_INTRO_PHASE;

int subScreenCondUnk6Is1(void)
{
    return GAME_AREA00_INTRO_PHASE == 1;
}
