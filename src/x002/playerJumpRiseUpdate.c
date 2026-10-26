// FUNC 800f262c 248 X002
// MATCHING 800f262c 248
extern unsigned char *DAT_8009c330;
extern volatile unsigned short PAD_PRESSED[];
extern unsigned short DAT_1f8003c6;
extern void playerSetJumpRiseVelocity(void);

void playerJumpRiseUpdate(unsigned char *o)
{
    unsigned short u;

    if (*(short *)(o + 0x7e) >= 0) {
        DAT_8009c330[8] = 1;
        o[6] = 2;
    }
    if ((PAD_PRESSED[0] & DAT_1f8003c6) != 0) {
        if (DAT_8009c330[8] != 0)
            return;
        u = *(short *)(DAT_8009c330 + 0x20) + 1;
        *(unsigned short *)(DAT_8009c330 + 0x20) = u;
        if (0xd < u) {
            DAT_8009c330[8] = 1;
            o[6] = 2;
        }
    } else {
        DAT_8009c330[8] = 1;
        if (4 < *(unsigned short *)(DAT_8009c330 + 0x20)) {
            o[6] = 2;
            return;
        }
        *(unsigned short *)(DAT_8009c330 + 0x20) = *(unsigned short *)(DAT_8009c330 + 0x20) + 1;
    }
    playerSetJumpRiseVelocity();
}
