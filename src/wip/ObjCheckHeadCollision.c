// FUNC 8003facc 196 MAIN0
extern short DAT_800a4580[];
extern short FUN_800408d8(int, int, int);

int ObjCheckHeadCollision(int p)
{
    short r;
    short y = *(short *)(p + 0x16);
    if (y < DAT_800a4580[0] - 0x90) {
        *(short *)(p + 0x16) = DAT_800a4580[0] - 0x90;
        return 1;
    }
    if ((r = FUN_800408d8(p, (short)(*(unsigned short *)(*(int *)(p + 0x40) + 2) + 4), (short)(y - 0x15))) != 0)
        return r;
    if ((r = FUN_800408d8(p, (short)(*(unsigned short *)(*(int *)(p + 0x40) + 2) - 4),
                          (short)(*(unsigned short *)(p + 0x16) - 0x15))) != 0)
        return r;
    return 0;
}
