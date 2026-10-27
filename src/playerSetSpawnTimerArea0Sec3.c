// FUNC 80122990 72 X000
// MATCHING 80122990 72
extern unsigned short GAME_SELECTED_SECTION;
extern unsigned char DAT_8009cda5;
extern unsigned short GAME_SELECTED_SPAWN_POINT;

void playerSetSpawnTimerArea0Sec3(char *o)
{
    if (GAME_SELECTED_SECTION == 3 && DAT_8009cda5 == 0xff && GAME_SELECTED_SPAWN_POINT == 0)
        *(short *)(o + 0x20) = 0x8c;
}
