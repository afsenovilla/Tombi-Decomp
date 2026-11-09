// VERSION SLPS_01144 slps01144 80027D58 152
// FLAGS -O2 -G0
#if defined(VERSION_SLPS_01144)
extern unsigned char D_8009EC27;
extern unsigned char D_8009EC28;
extern unsigned char GAME_PLAYER_MAX_HEALTH;
extern unsigned char GAME_PLAYER_HEALTH;
extern unsigned short PLAYER_HEALTH;
extern unsigned short D_800A7C12;

unsigned char increaseMaxHealth(void)
{
    unsigned char c;

    if (D_8009EC27 == 0) {
        c = GAME_PLAYER_MAX_HEALTH;
        if (c >= 8) {
            D_8009EC28++;
            goto done;
        }
    } else {
        c = GAME_PLAYER_MAX_HEALTH;
        if (c >= 16) goto done;
    }
    GAME_PLAYER_HEALTH = D_800A7C12 = PLAYER_HEALTH = GAME_PLAYER_MAX_HEALTH = c + 1;
done:
    return GAME_PLAYER_MAX_HEALTH;
}
#endif
