// FUNC 800ee670 24 D0SPSUB3
// MATCHING 800ee670 24
extern int GAME_AREA00_EVENT_CONTROL;

int subScreenCondFlag100(void)
{
    if (GAME_AREA00_EVENT_CONTROL & 0x100) return 1;
    return 0;
}
