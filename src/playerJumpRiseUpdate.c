// FUNC 800f262c 248 X000
// MATCHING 800f262c 248
extern unsigned char *PLAYER_EXT;
extern volatile unsigned short PAD_PRESSED[];
extern unsigned short BUTTON_MASK_B;
extern void playerSetJumpRiseVelocity(void);

void playerJumpRiseUpdate(unsigned char *o)
{
    unsigned short u;

    if (*(short *)(o + 0x7e) >= 0) {
        PLAYER_EXT[8] = 1;
        o[6] = 2;
    }
    if ((PAD_PRESSED[0] & BUTTON_MASK_B) != 0) {
        if (PLAYER_EXT[8] != 0)
            return;
        u = *(short *)(PLAYER_EXT + 0x20) + 1;
        *(unsigned short *)(PLAYER_EXT + 0x20) = u;
        if (0xd < u) {
            PLAYER_EXT[8] = 1;
            o[6] = 2;
        }
    } else {
        PLAYER_EXT[8] = 1;
        if (4 < *(unsigned short *)(PLAYER_EXT + 0x20)) {
            o[6] = 2;
            return;
        }
        *(unsigned short *)(PLAYER_EXT + 0x20) = *(unsigned short *)(PLAYER_EXT + 0x20) + 1;
    }
    playerSetJumpRiseVelocity();
}
