// FUNC 800587a8 388 MAIN0
extern unsigned char DAT_8009c980;
extern void FUN_800594e4(char *, short, short, unsigned short);
extern void AddDrawMode(int, int);

void FUN_800587a8(char *o, int px, int py)
{
    int idx, x;
    unsigned short c;
    idx = DAT_8009c980 - 1;
    if (idx < 10) {
        x = (short)px;
        c = **(unsigned short **)(DAT_8009c980 * 4 + *(int *)(o + 0x24) - 4);
    } else {
        FUN_800594e4(o, px - 5, py, **(unsigned short **)((idx / 10) * 4 + *(int *)(o + 0x24)));
        x = px + 5;
        c = **(unsigned short **)(((DAT_8009c980 - 1) % 10) * 4 + *(int *)(o + 0x24));
    }
    FUN_800594e4(o, x, py, c);
    FUN_800594e4(o, px, py - 4, **(unsigned short **)(o + 0x30));
    AddDrawMode(0x15, 1);
}
