// FUNC 8001be1c 208 MAIN0
// MATCHING 8001be1c 208
extern short *TASK_C;
extern int DAT_8009f7e4;
extern void drawLoadingSprites(int, int);

void displayLoadingScreen(void)
{
    short *o;
    short *p;
    short *q;
    if (DAT_8009f7e4 != 0) {
        if (DAT_8009f7e4 != 1)
            return;
    } else {
        o = TASK_C;
        o[0x31] = 0;
        o[0x30] = 0xf;
        o[0x32] = 0;
        drawLoadingSprites(o[0x31], 0);
        DAT_8009f7e4 = DAT_8009f7e4 + 1;
    }
    p = TASK_C;
    p[0x30] = p[0x30] - 1;
    if (p[0x30] == 0) {
        p[0x30] = 0xf;
        p[0x31] = p[0x31] ^ 1;
    }
    q = TASK_C;
    q[0x32] = (q[0x32] + 0xc) & 0xff;
    drawLoadingSprites(q[0x31], q[0x32]);
}
