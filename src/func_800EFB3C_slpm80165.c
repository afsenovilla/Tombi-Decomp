// VERSION SLPM_80165 x00 800EFB3C 192
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo version of playerJumpRiseUpdate (src/x018/playerJumpRiseUpdate.c): button 0x40 fixed, no hold
// limit, the rise end does not set the flag.
extern unsigned char *PLAYER_EXT;
extern volatile unsigned short PAD_PRESSED[];
extern void playerSetJumpRiseVelocity(void);

void playerJumpRiseUpdate(unsigned char *o)
{
    if (*(short *)(o + 0x7e) >= 0)
        o[6] = 2;
    if ((PAD_PRESSED[0] & 0x40) != 0) {
        if (PLAYER_EXT[8] != 0)
            return;
        *(unsigned short *)(PLAYER_EXT + 0x20) = *(unsigned short *)(PLAYER_EXT + 0x20) + 1;
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
#endif
