// FUNC 800f1550 24 D0SPSUB
// MATCHING 800f1550 24
extern int GAME_AREA00_EVENT_CONTROL;

int subScreenCondFlag100(void)
{
    if (GAME_AREA00_EVENT_CONTROL & 0x100) return 1;
    return 0;
}
