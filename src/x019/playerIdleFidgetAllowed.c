// FUNC 800f0b5c 204 X019
// MATCHING 800f0b5c 204
typedef struct { short x; short y; } Cam;
extern unsigned short GAME;
extern unsigned short GAME_SELECTED_SECTION;
extern unsigned char DAT_8009cdb5;
extern unsigned char DAT_8009d0ce;
extern Cam *PLAYER_H;

short playerIdleFidgetAllowed(void)
{
    short r = 0;
    switch (GAME) {
    case 1:
        if (GAME_SELECTED_SECTION != 1)
            r = 1;
        else if (DAT_8009cdb5)
            r = 1;
        break;
    case 9:
        if (GAME_SELECTED_SECTION != 1)
            r = 1;
        else if (DAT_8009d0ce == 0) {
            if (PLAYER_H->y >= 0x73)
                r = 1;
        } else if (PLAYER_H->y < 0x72)
            r = 1;
        break;
    default:
        r++;
        break;
    }
    return r;
}
