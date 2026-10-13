// FUNC 801306f0 176 X000
// MATCHING 801306f0 176
extern void FUN_8005f290(short *, int, int, int);
extern void unpackSpriteFrame(int, int, char *, int);
extern void loadTIM(char *, int, int, int, int);
extern int DAT_1f800350;
extern char DAT_800d7e28[];
void FUN_801306f0(int a, int b)
{
    short r[4];
    r[0] = b * 0x20 + 0x1c0;
    r[1] = 0xa0;
    r[2] = 0x20;
    r[3] = 0x60;
    FUN_8005f290(r, 0, 0, 0);
    unpackSpriteFrame(DAT_1f800350, (short)a, DAT_800d7e28, 0xa001c0);
    loadTIM(DAT_800d7e28, (short)(b * 0x20 + 0x1c1), 0xa0, 0xe0, 0x1f0);
}
