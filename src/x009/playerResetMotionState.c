// FUNC 800ee3d4 84 X009
// MATCHING 800ee3d4 84
extern unsigned char *PLAYER_EXT;
void playerResetMotionState(unsigned char *o)
{
    PLAYER_EXT[8] = 0;
    PLAYER_EXT[9] = 0;
    *(short *)(PLAYER_EXT + 0xc) = 0;
    *(unsigned short *)(PLAYER_EXT + 0x28) = 0xffff;
    *(unsigned short *)(PLAYER_EXT + 0x2a) = 0xffff;
    *(short *)(o + 0x80) = 0;
    *(short *)(o + 0x82) = 0;
    *(short *)(o + 0xb0) = 0;
    *(int *)(o + 0x84) = 0;
    *(int *)(o + 0x88) = 0;
    *(int *)(o + 0x8c) = 0;
}
