// FUNC 8003facc 196 MAIN0
extern short DAT_800a4580;
extern short FUN_800408d8(int, int, int);

int ObjCheckHeadCollision(int p)
{
    short s;
    int r;
    int y = *(short *)(p + 0x16);
    short d = DAT_800a4580;
    if (y < d - 0x90) {
        *(short *)(p + 0x16) = d - 0x90;
        r = 1;
    } else {
        s = FUN_800408d8(p, (short)(*(unsigned short *)(*(int *)(p + 0x40) + 2) + 4),
                         (short)(y - 0x15));
        r = s;
        if (r == 0) {
            s = FUN_800408d8(p, (short)(*(unsigned short *)(*(int *)(p + 0x40) + 2) - 4),
                             (short)(*(unsigned short *)(p + 0x16) - 0x15));
            r = s;
            if (r == 0)
                r = 0;
        }
    }
    return r;
}
