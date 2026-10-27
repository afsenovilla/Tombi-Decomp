// FUNC 80026f4c 156 MAIN0
// MATCHING 80026f4c 156
extern unsigned char DAT_8009d07f;
extern unsigned char GAME_PLAYER_MAX_HEALTH;
extern volatile unsigned char V_8009c971 asm("GAME_PLAYER_MAX_HEALTH");
extern unsigned char GAME_PLAYER_HEALTH;
extern unsigned char DAT_8009d080;
extern unsigned short PLAYER_HEALTH;
extern unsigned short DAT_800a60d2;

void increaseMaxHealth(void)
{
    unsigned short c;
    unsigned char d;
    if (DAT_8009d07f == 0) {
        c = GAME_PLAYER_MAX_HEALTH;
        if (c >= 8) {
            DAT_8009d080 = DAT_8009d080 + 1;
            goto done;
        }
    } else {
        c = GAME_PLAYER_MAX_HEALTH;
        if (c >= 16) goto done;
    }
    GAME_PLAYER_MAX_HEALTH = c + 1;
done:
    d = V_8009c971;
    c = d;
    PLAYER_HEALTH = c;
    DAT_800a60d2 = c;
    GAME_PLAYER_HEALTH = c;
}
