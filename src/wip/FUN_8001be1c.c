// FUNC 8001be1c 208 MAIN0
extern short *DAT_1f8001d4;
extern int DAT_8009f7e4;
extern void FUN_80021150(int, int);

void FUN_8001be1c(void)
{
    short *o;
    short *p;
    short *q;
    if (DAT_8009f7e4 != 0) {
        if (DAT_8009f7e4 != 1)
            return;
    } else {
        o = DAT_1f8001d4;
        o[0x31] = 0;
        o[0x30] = 0xf;
        o[0x32] = 0;
        FUN_80021150(o[0x31], 0);
        DAT_8009f7e4 = DAT_8009f7e4 + 1;
    }
    p = DAT_1f8001d4;
    p[0x30] = p[0x30] - 1;
    if (p[0x30] == 0) {
        p[0x30] = 0xf;
        p[0x31] = p[0x31] ^ 1;
    }
    q = DAT_1f8001d4;
    q[0x32] = (q[0x32] + 0xc) & 0xff;
    FUN_80021150(q[0x31], q[0x32]);
}
