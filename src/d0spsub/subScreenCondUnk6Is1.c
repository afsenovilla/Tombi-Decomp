// FUNC 800f1538 24 D0SPSUB
// MATCHING 800f1538 24
extern unsigned char GAME_AREA00_INTRO_PHASE;

int subScreenCondUnk6Is1(void)
{
    return GAME_AREA00_INTRO_PHASE == 1;
}
