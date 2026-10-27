// FUNC 800ee9c8 24 D0SPSUB3
// MATCHING 800ee9c8 24
extern int GAME_AREA00_EVENT_CONTROL;

int subScreenCondFlag40(void)
{
    if (GAME_AREA00_EVENT_CONTROL & 0x40) return 1;
    return 0;
}
