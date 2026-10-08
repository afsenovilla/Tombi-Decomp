// FUNC 800587a8 388 MAIN0
extern unsigned char DAT_8009c980;
extern void FUN_800594e4(char *, short, short, unsigned short);
extern void AddDrawMode(int, int);

void FUN_800587a8(char *o, int px, int py)
{
    unsigned char *n;
    int x, y; x = px; y = py;
    n = &DAT_8009c980;
    if (*n - 1 >= 10) {
        FUN_800594e4(o, px - 5, py, **(unsigned short **)(*(int *)(o + 0x24) + ((*n - 1) / 10) * 4));
        FUN_800594e4(o, px + 5, py, **(unsigned short **)(*(int *)(o + 0x24) + ((*n - 1) % 10) * 4));
    } else {
        FUN_800594e4(o, px, py, **(unsigned short **)(*(int *)(o + 0x24) + *n * 4 - 4));
    }
    FUN_800594e4(o, x, y - 4, **(unsigned short **)(o + 0x30));
    AddDrawMode(0x15, 1);
}
